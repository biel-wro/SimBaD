#include "dataframe_tracker.hpp"

#include <boost/intrusive/unordered_set.hpp>

#include "interface/attribute.hpp"
#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_vector.hpp"

BEGIN_NAMESPACE_CORE

struct dataframe_tracker::node : public dataframe_tracker::hook_type
{
  attribute const & get_attribute(std::size_t idx) const
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

std::size_t dataframe_tracker::node_equal::
operator()(const dataframe_tracker::node &lhs,
           const dataframe_tracker::node &rhs) const
{
  for(std::size_t idx = 0; idx < m_key_size; ++idx)
    if(lhs.get_attribute(idx) != rhs.get_attribute(idx))
      return false;
  return true;
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

void dataframe_tracker::feed_record(const attribute_list &input) {}

END_NAMESPACE_CORE
