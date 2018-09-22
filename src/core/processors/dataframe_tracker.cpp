#include "dataframe_tracker.hpp"

#include "interface/attribute.hpp"

#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_vector.hpp"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>
#include <boost/pool/pool.hpp>

#include <algorithm>

BEGIN_NAMESPACE_CORE

struct set_type : public boost::intrusive::unordered_set<
                      tracker_record,                                       //
                      boost::intrusive::hash<tracker_record::key_hasher>,   //
                      boost::intrusive::equal<tracker_record::key_equaler>, //
                      boost::intrusive::compare_hash<true>,                 //
                      boost::intrusive::power_2_buckets<true>               //
                      >
{
};

struct bucket_type : public set_type::bucket_type
{
};

dataframe_tracker::dataframe_tracker(std::size_t record_size)
    : m_record_size(record_size),
      m_bucket_count(minimal_bucket_count),
      m_buckets(new bucket_type[minimal_bucket_count]),
      m_attribute_set(
          set_type::bucket_traits(m_buckets.get(), minimal_bucket_count),
          tracker_record::key_hasher{}, tracker_record::key_equaler{}),
      m_record_pool(sizeof(attribute) * m_record_size)
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

void dataframe_tracker::realloc_buckets(std::size_t new_bucket_count)
{
  std::unique_ptr<bucket_type[]> new_buckets(new bucket_type[new_bucket_count]);

  m_attribute_set.rehash(
      set_type::bucket_traits(new_buckets.get(), new_bucket_count));
  std::swap(m_buckets, new_buckets);
  m_bucket_count = new_bucket_count;
}

void dataframe_tracker::rehash_if_needed()
{
  double fill_factor_upper_bound = 0.7;
  std::size_t current_size = m_attribute_set.size();

  if(current_size > m_bucket_count * fill_factor_upper_bound)
  {
    // std::cerr << "REALLOC:"
    //          << " m_bucket:" << m_bucket_count
    //          << " current_size:" << current_size << std::endl;
    // std::cerr.flush();

    realloc_buckets(m_bucket_count * 2);

    return;
  }

  /*
  if(current_size * 2 < m_bucket_count * fill_factor_upper_bound)
  {
    std::cerr << "ZMNIEJSZANIE:"
              << " m_bucket:" << m_bucket_count << std::endl;
    std::cerr << "currnent_size:" << current_size << std::endl;
    std::cerr.flush();

    realloc_buckets(std::max(m_bucket_count / 2, minimal_bucket_count));
    return;
  }*/
}

END_NAMESPACE_CORE
