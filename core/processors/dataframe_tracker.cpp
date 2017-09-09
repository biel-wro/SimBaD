#include "dataframe_tracker.hpp"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>

#include "interface/attribute.hpp"
#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_vector.hpp"

BEGIN_NAMESPACE_CORE

struct dataframe_tracker::node : public dataframe_tracker::hook_type
{
  static std::vector<attribute>
  make_attributes(attribute_list const &other_attributes,
                  std::vector<std::size_t> const &mapping)
  {
    std::size_t const size = mapping.size();
    std::vector<attribute> attributes(size);
    for(std::size_t my_idx = 0; my_idx < size; ++my_idx)
    {
      std::size_t other_idx = mapping[my_idx];
      attributes[my_idx] = other_attributes[other_idx];
    }
    return attributes;
  }

  node(attribute_list const &other_attributes,
       std::vector<std::size_t> const &mapping)
      : m_attributes(make_attributes(other_attributes, mapping))
  {
  }
  attribute const &get_attribute(std::size_t idx) const
  {
    assert(m_attributes.size() > idx);
    return m_attributes[idx];
  }
  std::vector<attribute> const m_attributes;
};

std::size_t dataframe_tracker::node_hash::
operator()(const dataframe_tracker::node &n) const
{
  std::size_t seed = 0;
  for(std::size_t idx = 0; idx < m_key_size; ++idx)
    boost::hash_combine(seed, n.get_attribute(idx));
  return seed;
}

std::size_t dataframe_tracker::node_hash::
operator()(attribute const &attr) const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, attr);
  return seed;
}

bool dataframe_tracker::node_equal::
operator()(const dataframe_tracker::node &lhs,
           const dataframe_tracker::node &rhs) const
{
  for(std::size_t idx = 0; idx < m_key_size; ++idx)
    if(lhs.get_attribute(idx) != rhs.get_attribute(idx))
      return false;
  return true;
}

bool dataframe_tracker::node_equal::
operator()(dataframe_tracker::node const &node, attribute const &attr) const
{
  assert(1 == m_key_size);
  return node.get_attribute(0) == attr;
}
bool dataframe_tracker::node_equal::
operator()(attribute const &attr, dataframe_tracker::node const &node) const
{
  return operator()(node, attr);
}

bool dataframe_tracker::node_equal::operator()(const attribute &lhs,
                                               const attribute &rhs) const
{
  return lhs == rhs;
}

dataframe_tracker::dataframe_tracker(attribute_description const &description,
                                     std::vector<std::string> const &key_names,
                                     std::vector<std::string> const &val_names)
    : m_buckets(new bucket_type[initial_bucket_count]),
      m_attribute_set(
          set_type::bucket_traits(m_buckets.get(), initial_bucket_count),
          node_hash{key_names.size()}, node_equal{key_names.size()}),
      m_attribute_description(
          attribute_description::mapped_from(description, key_names)),
      m_to_outer_indices(m_attribute_description.lin_mapping_from(description))
{
  m_attribute_description.add_attributes(description, val_names);
}

dataframe_tracker::node_hash dataframe_tracker::hasher() const
{
  return m_attribute_set.hash_function();
}
dataframe_tracker::node_equal dataframe_tracker::equaler() const
{
  return m_attribute_set.key_eq();
}
boost::optional<dataframe_tracker::const_iterator>
dataframe_tracker::find(const attribute &attr) const
{
  const_iterator it = m_attribute_set.find(attr, hasher(), equaler());
  if(it == m_attribute_set.end())
    return boost::none;
  return it;
}

END_NAMESPACE_CORE
