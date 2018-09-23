#include "dataframe_tracker.hpp"

#include "interface/attribute.hpp"

#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_vector.hpp"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>
#include <boost/pool/pool.hpp>

#include <algorithm>

BEGIN_NAMESPACE_CORE

namespace
{
struct record_disposer
{
  record_disposer(dataframe_tracker &tracker) : m_tracker(tracker) {}
  void operator()(tracker_record *record_ptr) const
  {
    m_tracker.destroy_record(record_ptr);
  }
  dataframe_tracker &m_tracker;
};
}

dataframe_tracker::dataframe_tracker(std::size_t record_length)
    : m_record_length(record_length),
      m_record_bytes(tracker_record::record_bytes(record_length)),
      m_record_pool(m_record_bytes),

      m_bucket_count(minimal_bucket_count),
      m_buckets(new bucket_type[minimal_bucket_count]),
      m_attribute_set(
          set_type::bucket_traits(m_buckets.get(), minimal_bucket_count),
          tracker_record::key_hasher{}, tracker_record::key_equaler{})
{
}

dataframe_tracker::~dataframe_tracker()
{
  // de-init is enough as memory will be freed when pool is destroyed
  m_attribute_set.clear_and_dispose([this](tracker_record *ptr) {
    tracker_record::deinitialize(ptr, m_record_length);
  });
}

tracker_record *dataframe_tracker::create_record()
{
  void *memory_ptr = m_record_pool.malloc();
  tracker_record *record_ptr =
      tracker_record::initialize(memory_ptr, m_record_length);
  return record_ptr;
}

tracker_record *dataframe_tracker::create_record(attribute const &key)
{
  void *memory_ptr = m_record_pool.malloc();
  tracker_record *record_ptr =
      tracker_record::initialize(memory_ptr, m_record_length, key);
  return record_ptr;
}
template <class AttributeList, class RecordIdxIterator>
tracker_record *dataframe_tracker::create_record(AttributeList &&list,
                                                 RecordIdxIterator beg_idx,
                                                 RecordIdxIterator last_idx)
{
  assert(std::distance(beg_idx, last_idx) == m_record_length);
  void *memory_ptr = m_record_pool.malloc();
  tracker_record *record_ptr =
      tracker_record::initialize(memory_ptr, list, beg_idx, last_idx);
  return record_ptr;
}

void dataframe_tracker::destroy_record(tracker_record *record_ptr)
{
  void *memory_ptr = tracker_record::deinitialize(record_ptr, m_record_length);
  m_record_pool.free(memory_ptr);
}

std::pair<dataframe_tracker::iterator, bool>
dataframe_tracker::insert_check(attribute const &key,
                                insert_commit_data &commit_data)
{
  std::pair<iterator, bool> result =
      attribute_set().insert_check(key, tracker_record::key_hasher(),
                                   tracker_record::key_equaler(), commit_data);
  return result;
}

dataframe_tracker::iterator
dataframe_tracker::insert_commit(tracker_record *record_ptr,
                                 insert_commit_data const &commit_data)
{
  iterator it = attribute_set().insert_commit(*record_ptr, commit_data);
  rehash_if_needed();
  return it;
}
dataframe_tracker::iterator
dataframe_tracker::insert_commit(attribute const &key,
                                 insert_commit_data const &commit_data)
{
  tracker_record *record_ptr = create_record();
  record_ptr->get(0) = key;

  return insert_commit(record_ptr, commit_data);
}

template <class AttributeList, class RecordIdxIterator>
dataframe_tracker::iterator dataframe_tracker::insert_commit(
    AttributeList &&list, RecordIdxIterator first_idx,
    RecordIdxIterator last_idx, insert_commit_data const &commit_data)
{
  tracker_record *record_ptr =
      create_record(std::forward<AttributeList>(list), first_idx, last_idx);

  iterator it = insert_commit(record_ptr, commit_data);
  return it;
}

std::pair<dataframe_tracker::iterator, bool>
dataframe_tracker::insert(tracker_record *record_ptr)
{
  std::pair<iterator, bool> result = m_attribute_set.insert(*record_ptr);
  rehash_if_needed();
  return result;
}

template <class AttributeList, class RecordIdxIterator>
std::pair<dataframe_tracker::iterator, bool>
dataframe_tracker::insert(AttributeList &&list, RecordIdxIterator first_idx,
                          RecordIdxIterator last_idx)
{
  tracker_record *record_ptr = create_record(list, first_idx, last_idx);
  std::pair<iterator, bool> result = m_attribute_set.insert(*record_ptr);

  if(!result.second)
    destroy_record(record_ptr);
  rehash_if_needed();
  return result;
}
std::pair<dataframe_tracker::iterator, bool>
dataframe_tracker::insert(const simbad::core::attribute &key)
{
  tracker_record *record_ptr = create_record(key);
  std::pair<iterator, bool> result = m_attribute_set.insert(*record_ptr);

  if(!result.second)
    destroy_record(record_ptr);
  rehash_if_needed();
  return result;
}

template <class AttributeList, class RecordIdxIterator>
bool dataframe_tracker::update(AttributeList &&list,
                               RecordIdxIterator first_idx,
                               RecordIdxIterator last_idx)
{
  attribute const &key_attribute = list[*first_idx];

  iterator it =
      m_attribute_set.find(key_attribute, tracker_record::key_hasher(),
                           tracker_record::key_equaler());
  if(m_attribute_set.end() == it)
    return false;

  tracker_record &record = *it;
  record.set(1, list, std::next(first_idx), last_idx);
  return true;
}

namespace
{
struct attribute_list_view final : public attribute_list
{
  attribute_list_view(tracker_record const &record) : m_record_ptr(&record) {}
  attribute get_attribute(std::size_t idx) const override
  {
    return m_record_ptr->get(idx);
  }

private:
  tracker_record const *m_record_ptr;
};
}

void dataframe_tracker::visit_records(record_visitor visitor) const
{
  for(tracker_record const &record : m_attribute_set)
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

void dataframe_tracker::erase(const_iterator it)
{
  m_attribute_set.erase_and_dispose(it, record_disposer(*this));
}

void dataframe_tracker::erase(attribute const &key)
{
  m_attribute_set.erase_and_dispose(key,                             //
                                    m_attribute_set.hash_function(), //
                                    m_attribute_set.key_eq(),        //
                                    record_disposer(*this)           //
  );
}

std::size_t dataframe_tracker::size() const { return m_attribute_set.size(); }
std::size_t dataframe_tracker::record_length() const { return m_record_length; }

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
}

#define DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION_ATTR(AttrList, Iter)          \
  template tracker_record *dataframe_tracker::create_record<AttrList, Iter>(   \
      AttrList, Iter, Iter);                                                   \
  template dataframe_tracker::iterator                                         \
  dataframe_tracker::insert_commit<AttrList, Iter>(                            \
      AttrList, Iter, Iter, insert_commit_data const &);                       \
  template std::pair<dataframe_tracker::iterator, bool>                        \
      dataframe_tracker::insert<AttrList, Iter>(AttrList, Iter, Iter);         \
  template bool dataframe_tracker::update<AttrList, Iter>(AttrList, Iter, Iter);

#define DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION(Iter)                         \
  DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION_ATTR(attribute_list const &, Iter); \
  DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION_ATTR(attribute_list &, Iter);       \
  DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION_ATTR(attribute_list &&, Iter);

DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION(std::vector<std::size_t>::iterator);
DATAFRAME_TRACKER_EXPLICIT_INSTANTIATION(
    std::vector<std::size_t>::const_iterator);

END_NAMESPACE_CORE
