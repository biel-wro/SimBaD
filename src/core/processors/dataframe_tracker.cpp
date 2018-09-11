#include "dataframe_tracker.hpp"

#include "interface/attribute.hpp"
#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_vector.hpp"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>

#include <algorithm>

BEGIN_NAMESPACE_CORE

dataframe_tracker::record::record(std::size_t s)
    : m_attributes(new attribute[s])
{
}
dataframe_tracker::record::record(std::initializer_list<attribute> il)
{
  std::move(il.begin(), il.end(), m_attributes.get());
}
dataframe_tracker::record::record(
    attribute_list const &values,
    dataframe_tracker::index_vector const &indices)
    : record(indices.size())
{
  update(values, indices);
}
attribute const &dataframe_tracker::record::operator[](std::size_t idx) const
{
  return m_attributes[idx];
}

attribute &dataframe_tracker::record::operator[](std::size_t idx)
{
  return m_attributes[idx];
}

attribute &dataframe_tracker::record::get(std::size_t idx)
{
  return m_attributes[idx];
}

void dataframe_tracker::record::update(attribute_list const &new_values,
                                       index_vector const &mapping,
                                       std::size_t skip_size)
{
  for(std::size_t i = skip_size, size = mapping.size(); i < size; ++i)
  {
    std::size_t outer_idx = mapping[i];
    m_attributes[i] = new_values[outer_idx];
  }
}

std::size_t dataframe_tracker::node_hash::
operator()(const dataframe_tracker::record &n) const
{
  std::size_t seed = 0;
  for(std::size_t idx = 0; idx < m_key_size; ++idx)
    boost::hash_combine(seed, n[idx]);
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
operator()(const dataframe_tracker::record &lhs,
           const dataframe_tracker::record &rhs) const
{
  for(std::size_t idx = 0; idx < m_key_size; ++idx)
    if(lhs[idx] != rhs[idx])
      return false;
  return true;
}

bool dataframe_tracker::node_equal::
operator()(dataframe_tracker::record const &node, attribute const &attr) const
{
  assert(1 == m_key_size);
  return node[0] == attr;
}
bool dataframe_tracker::node_equal::
operator()(attribute const &attr, dataframe_tracker::record const &node) const
{
  return operator()(node, attr);
}

bool dataframe_tracker::node_equal::operator()(const attribute &lhs,
                                               const attribute &rhs) const
{
  return lhs == rhs;
}

dataframe_tracker::dataframe_tracker(std::size_t record_size,
                                     std::size_t key_size)
    : m_record_size(record_size),
      m_key_size(key_size),
      m_buckets(new bucket_type[initial_bucket_count]),
      m_attribute_set(
          set_type::bucket_traits(m_buckets.get(), initial_bucket_count),
          node_hash{key_size}, node_equal{key_size})
{
}

dataframe_tracker::~dataframe_tracker()
{
  m_attribute_set.clear_and_dispose(std::default_delete<record>());
}

namespace
{
struct attribute_list_view final : public attribute_list
{
  using node_ref = dataframe_tracker::record const &;
  attribute_list_view(node_ref ptr) : m_node_ref(ptr) {}
  attribute get_attribute(std::size_t idx) const override
  {
    return m_node_ref[idx];
  }

private:
  node_ref m_node_ref;
};
}

void dataframe_tracker::update(attribute_list const &values,
                               dataframe_tracker::index_vector const &indices)
{
  insert_commit_data commit_data;
  iterator it;
  bool ok;
  std::tie(it, ok) = insert_check(values, indices.begin(),
                                  indices.begin() + m_key_size, commit_data);
  if(!ok)
    return;

  insert_commit(values, indices.begin(), indices.end(), commit_data);
  rehash_if_needed();
}

void dataframe_tracker::visit_records(attribute_visitor visitor) const
{
  for(record const &record : m_attribute_set)
  {
    attribute_list_view view(record);
    visitor(view);
  }
}

const dataframe_tracker::set_type &dataframe_tracker::attribute_set() const
{
  return m_attribute_set;
}

dataframe_tracker::set_type &dataframe_tracker::attribute_set()
{
  return m_attribute_set;
}

std::pair<dataframe_tracker::iterator, bool> dataframe_tracker::insert_check(
    attribute const &key, dataframe_tracker::insert_commit_data &commit_data)
{
  assert(m_key_size == 1);
  std::pair<iterator, bool> result =
      m_attribute_set.insert_check(key,                             //
                                   m_attribute_set.hash_function(), //
                                   m_attribute_set.key_eq(),        //
                                   commit_data                      //
      );
  return result;
}

dataframe_tracker::iterator dataframe_tracker::insert_commit(
    const attribute &key,
    const dataframe_tracker::insert_commit_data &commit_data)
{
  assert(m_key_size == 1);
  std::unique_ptr<record> node_ptr(new record(m_record_size));
  node_ptr->get(0) = key;

  iterator it = attribute_set().insert_commit(*node_ptr.release(), commit_data);

  return it;
}

std::pair<dataframe_tracker::iterator, bool>
dataframe_tracker::insert(const attribute &key)
{
  insert_commit_data commit_data;
  std::pair<iterator, bool> check_result = insert_check(key, commit_data);
  if(!check_result.second)
    return std::make_pair(check_result.first, false);
  iterator it = insert_commit(key, commit_data);
  rehash_if_needed();
  return std::make_pair(it, true);
}

void dataframe_tracker::erase(dataframe_tracker::const_iterator it)
{
  m_attribute_set.erase_and_dispose(it, std::default_delete<record>());
}

void dataframe_tracker::erase(attribute const &key)
{
  m_attribute_set.erase_and_dispose(key,                             //
                                    m_attribute_set.hash_function(), //
                                    m_attribute_set.key_eq(),        //
                                    std::default_delete<record>()    //
  );
}

std::size_t dataframe_tracker::size() const { return m_attribute_set.size(); }
std::size_t dataframe_tracker::record_size() const { return m_record_size; }
std::size_t dataframe_tracker::key_size() const { return m_key_size; }
void dataframe_tracker::rehash_if_needed()
{
  std::size_t suggested_size =
      set_type::suggested_upper_bucket_count(m_attribute_set.size());

  if(m_attribute_set.bucket_count() > suggested_size)
    return;

  std::unique_ptr<bucket_type[]> new_buckets(new bucket_type[suggested_size]);
  m_attribute_set.rehash(
      set_type::bucket_traits(new_buckets.get(), suggested_size));
  std::swap(m_buckets, new_buckets);
}

END_NAMESPACE_CORE
