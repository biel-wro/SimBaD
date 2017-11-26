#include "dataframe_tracker.hpp"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>

#include "interface/attribute.hpp"
#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_vector.hpp"

BEGIN_NAMESPACE_CORE

std::vector<attribute> dataframe_tracker::node::make_attributes(
    attribute_list const &other_attributes,
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

dataframe_tracker::node::node(attribute_list const &other_attributes,
                              std::vector<std::size_t> const &mapping)
    : m_attributes(make_attributes(other_attributes, mapping))
{
}

attribute const &dataframe_tracker::node::get_attribute(std::size_t idx) const
{
  assert(m_attributes.size() > idx);
  return m_attributes[idx];
}

void dataframe_tracker::node::update(const attribute_list &new_values,
                                     const std::vector<std::size_t> &mapping,
                                     std::size_t key_size)
{
  for(std::size_t i = key_size; i < mapping.size(); ++i)
  {
    std::size_t outer_idx = mapping[i];
    m_attributes[i] = new_values[outer_idx];
  }
}

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
  std::size_t hash(hash_seed);
  boost::hash_combine(hash, attr);
  return hash;
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
      m_key_size(key_names.size()),
      m_attribute_set(
          set_type::bucket_traits(m_buckets.get(), initial_bucket_count),
          node_hash{key_names.size()}, node_equal{key_names.size()}),
      m_attribute_description(
          attribute_description::mapped_from(description, key_names)),
      m_to_outer_indices(m_attribute_description.lin_mapping_from(description))
{
  m_attribute_description.add_attributes(description, val_names);
}

dataframe_tracker::~dataframe_tracker()
{
  m_attribute_set.clear_and_dispose(std::default_delete<node>());
}

dataframe_tracker::node_hash dataframe_tracker::hasher() const
{
  return m_attribute_set.hash_function();
}
dataframe_tracker::node_equal dataframe_tracker::equaler() const
{
  return m_attribute_set.key_eq();
}

namespace
{
struct list_hasher
{
  std::vector<std::size_t> const &m_indices;
  list_hasher(std::vector<size_t> const &ref) : m_indices(ref) {}
  std::size_t operator()(attribute_list const &attributes) const
  {
    std::size_t hash(dataframe_tracker::hash_seed);

    for(std::size_t idx : m_indices)
      boost::hash_combine(hash, attributes[idx]);

    return hash;
  }
};

struct list_equaler
{
  std::vector<std::size_t> const &m_indices;
  std::size_t m_key_size;
  list_equaler(std::vector<std::size_t> const &indices, std::size_t key_size)
      : m_indices(indices), m_key_size(key_size)
  {
  }
  bool operator()(dataframe_tracker::node const &lhs,
                  attribute_list const &rhs) const
  {
    for(std::size_t idx = 0; idx < m_key_size; ++idx)
      if(lhs.get_attribute(idx) != rhs[idx])
        return false;
    return true;
  }
  bool operator()(attribute_list const &lhs,
                  dataframe_tracker::node const &rhs) const
  {
    return operator()(rhs, lhs);
  }
};
}

std::pair<dataframe_tracker::iterator, bool>
dataframe_tracker::insert_check(attribute_list const &attributes,
                                insert_commit_data &commit_data)
{
  std::pair<iterator, bool> result = m_attribute_set.insert_check(
      attributes, list_hasher(m_to_outer_indices),
      list_equaler(m_to_outer_indices, equaler().m_key_size), commit_data);

  return result;
}

dataframe_tracker::iterator dataframe_tracker::insert_commit(
    const attribute_list &attributes,
    const dataframe_tracker::insert_commit_data &commit_data)
{
  std::unique_ptr<node> node_ptr(new node(attributes, m_to_outer_indices));
  iterator it = m_attribute_set.insert_commit(*node_ptr.release(), commit_data);
  return it;
}

void dataframe_tracker::update(attribute_list const &attributes)
{
  iterator it;
  bool insertable;
  insert_commit_data commit_data;
  std::tie(it, insertable) = insert_check(attributes, commit_data);

  if(insertable)
    return (void)insert_commit(attributes, commit_data);
}
dataframe_tracker::const_iterator
dataframe_tracker::find(attribute const &attr) const
{
  const_iterator it = m_attribute_set.find(attr, hasher(), equaler());
  return it;
}

END_NAMESPACE_CORE
