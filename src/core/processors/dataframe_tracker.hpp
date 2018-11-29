#ifndef DATAFRAME_TRACKER_HPP
#define DATAFRAME_TRACKER_HPP
#include "interface/interface_fwd.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/dataframe.hpp"
#include "processors/tracker_record.hpp"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/pool/object_pool.hpp>

#include <cstddef>
#include <memory>

BEGIN_NAMESPACE_CORE

class dataframe_tracker
{
protected:
public:
  using set_type = boost::intrusive::unordered_set<
      tracker_record,                                       //
      boost::intrusive::hash<tracker_record::key_hasher>,   //
      boost::intrusive::equal<tracker_record::key_equaler>, //
      boost::intrusive::compare_hash<true>,                 //
      boost::intrusive::power_2_buckets<true>               //
      >;

  using bucket_type = set_type::bucket_type;
  using iterator = set_type::iterator;
  using const_iterator = set_type::const_iterator;
  using insert_commit_data = set_type::insert_commit_data;

  using record_visitor = std::function<void(attribute_list const &)>;

  static constexpr std::size_t minimal_bucket_count = 1024;
  explicit dataframe_tracker(std::size_t record_length);
  ~dataframe_tracker();

  tracker_record *create_record();
  tracker_record *create_record(attribute const &key);
  template <class AttributeList, class RecordIdxIterator>
  tracker_record *create_record(AttributeList &&list, RecordIdxIterator beg_idx,
                                RecordIdxIterator last_idx);
  void destroy_record(tracker_record *record_ptr);

  std::pair<iterator, bool> insert_check(attribute const &key,
                                         insert_commit_data &commit_data);

  iterator insert_commit(tracker_record *record_ptr,
                         insert_commit_data const &commit_data);

  iterator insert_commit(attribute const &key,
                         insert_commit_data const &commit_data);

  template <class AttributeList, class RecordIdxIterator>
  iterator insert_commit(AttributeList &&list, RecordIdxIterator first_idx,
                         RecordIdxIterator last_idx,
                         insert_commit_data const &commit_data);

  std::pair<iterator, bool> insert(tracker_record *record_ptr);
  template <class AttributeList, class RecordIdxIterator>
  std::pair<iterator, bool> insert(AttributeList &&list,
                                   RecordIdxIterator first_idx,
                                   RecordIdxIterator last_idx);
  std::pair<iterator, bool> insert(attribute const &key);

  template <class AttributeList, class RecordIdxIterator>
  bool update(AttributeList &&list, RecordIdxIterator first_idx,
              RecordIdxIterator last_idx);

  void visit_records(record_visitor visitor) const;

  iterator find(attribute const &key);


  void erase(const_iterator it);
  void erase(attribute const &key);

  set_type const &attribute_set() const;
  set_type &attribute_set();

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  std::size_t size() const;
  std::size_t record_length() const;

protected:
  void realloc_buckets(std::size_t new_bucket_count);
  void rehash_if_needed();

private:
  std::size_t const m_record_length;
  std::size_t const m_record_bytes;

  boost::pool<boost::default_user_allocator_malloc_free> m_record_pool;

  std::size_t m_bucket_count;
  std::unique_ptr<bucket_type[]> m_buckets;
  set_type m_attribute_set;
};
END_NAMESPACE_CORE

#endif // DATAFRAME_TRACKER_HPP
