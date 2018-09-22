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

public:
  using attribute_visitor = std::function<void(attribute_list const &)>;

  static constexpr std::size_t minimal_bucket_count = 1024;
  dataframe_tracker(std::size_t record_size);
  ~dataframe_tracker();

  void update(attribute_list const &values, index_vector const &indices);
  void visit_records(attribute_visitor visitor) const;

  set_type const &attribute_set() const;
  set_type &attribute_set();

  std::pair<iterator, bool> insert_check(attribute const &key,
                                         insert_commit_data &commit_data);
  iterator insert_commit(attribute const &key,
                         insert_commit_data const &commit_data);

  template <class KeyIdxIterator>
  std::pair<iterator, bool>
  insert_check(attribute_list const &list, KeyIdxIterator first_key_idx,
               KeyIdxIterator last_key_idx, insert_commit_data &commit_data)
  {
    std::pair<iterator, bool> result = attribute_set().insert_check(
        list,                                           //
        make_list_hasher(first_key_idx, last_key_idx),  //
        make_list_equaler(first_key_idx, last_key_idx), //
        commit_data                                     //
    );
    return result;
  }
  template <class AttributeList, class RecordIdxIterator>
  iterator insert_commit(AttributeList &&list, RecordIdxIterator first_idx,
                         RecordIdxIterator last_idx,
                         insert_commit_data &commit_data)
  {
    std::unique_ptr<record> node_ptr(
        new record(std::forward<AttributeList>(list), first_idx, last_idx));
    iterator it =
        attribute_set().insert_commit(*node_ptr.release(), commit_data);
    rehash_if_needed();
    return it;
  }

  std::pair<iterator, bool> insert(attribute const &key);
  void erase(const_iterator it);
  void erase(attribute const &key);

  std::size_t size() const;
  std::size_t record_size() const;
  std::size_t key_size() const;

  void realloc_buckets(std::size_t new_bucket_count);
  void rehash_if_needed();

private:
  std::size_t const m_record_size;
  std::size_t m_bucket_count;
  std::unique_ptr<bucket_type[]> m_buckets;

  boost::pool<boost::default_user_allocator_malloc_free> m_record_pool;
  set_type m_attribute_set;

  //  boost::object_pool<
};
END_NAMESPACE_CORE

#endif // DATAFRAME_TRACKER_HPP
