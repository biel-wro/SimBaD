#ifndef DATAFRAME_TRACKER_HPP
#define DATAFRAME_TRACKER_HPP
#include "interface/interface_fwd.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/dataframe.hpp"

#include <boost/intrusive/unordered_set.hpp>

#include <cstddef>
#include <memory>

BEGIN_NAMESPACE_CORE

class dataframe_tracker
{
protected:
  using hook_type = boost::intrusive::unordered_set_base_hook<
      boost::intrusive::store_hash<true>>;

public:
  using index_vector = std::vector<std::size_t>;

  struct record : public dataframe_tracker::hook_type
  {
    explicit record(std::size_t record_size);
    explicit record(std::initializer_list<attribute> il);
    template <class RecordIdxIterator>
    record(attribute_list const &list, RecordIdxIterator first_idx,
           RecordIdxIterator last_idx)
        : record(std::distance(first_idx, last_idx))
    {
      std::transform(first_idx, last_idx, m_attributes.get(),
                     [&list](std::size_t idx) { return list[idx]; });
    }
    template <class RecordIdxIterator>
    record(attribute_list &&list, RecordIdxIterator first_idx,
           RecordIdxIterator last_idx)
        : record(std::distance(first_idx, last_idx))
    {
      std::transform(first_idx, last_idx, m_attributes.get(),
                     [&list](std::size_t idx) { return std::move(list[idx]); });
    }

    record(attribute_list const &values, index_vector const &indices);
    attribute const &operator[](std::size_t idx) const;
    attribute &operator[](std::size_t idx);

    attribute const &get(std::size_t idx) const;
    attribute &get(std::size_t idx);

    void update(attribute_list const &new_values, index_vector const &mapping,
                std::size_t skip_size = 0);
    template <class RecordIdxIt>
    void update(attribute_list const &new_values, RecordIdxIt first_idx,
                RecordIdxIt last_idx, std::size_t offset = 0)
    {
      std::size_t idx = offset;
      for(RecordIdxIt it = first_idx; it != last_idx; ++it, ++idx)
      {
        std::size_t source_idx = *it;
        m_attributes[idx] = new_values[source_idx];
      }
    }

  private:
    std::unique_ptr<attribute[]> m_attributes;
  };

  struct node_hash
  {
    node_hash(std::size_t key_size) : m_key_size(key_size) {}
    std::size_t operator()(dataframe_tracker::record const &n) const;
    std::size_t operator()(attribute const &attr) const;
    std::size_t const m_key_size;
  };
  struct node_equal
  {
    node_equal(std::size_t key_size) : m_key_size(key_size) {}
    std::size_t const m_key_size;
    bool operator()(record const &lhs, record const &rhs) const;
    bool operator()(record const &record, attribute const &attr) const;
    bool operator()(attribute const &attr, record const &record) const;
    bool operator()(attribute const &lhs, attribute const &rhs) const;
  };

  template <class Iterator> struct list_hasher
  {
    Iterator const m_begin_indices, m_end_indices;
    list_hasher(Iterator first, Iterator last)
        : m_begin_indices(first), m_end_indices(last)
    {
    }
    std::size_t operator()(attribute_list const &attributes) const
    {
      std::size_t hash(dataframe_tracker::hash_seed);
      std::for_each(m_begin_indices, m_end_indices, [&](std::size_t idx) {
        boost::hash_combine(hash, attributes[idx]);
      });

      return hash;
    }
  };
  template <class Iterator>
  static list_hasher<Iterator> make_list_hasher(Iterator first, Iterator last)
  {
    return list_hasher<Iterator>(first, last);
  }

  template <class Iterator> struct list_equaler
  {
    Iterator const m_begin_indices, m_end_indices;
    list_equaler(Iterator begin_indices, Iterator end_indices)
        : m_begin_indices(begin_indices), m_end_indices(end_indices)
    {
    }
    bool operator()(dataframe_tracker::record const &lhs,
                    attribute_list const &rhs) const
    {
      return std::all_of(m_begin_indices, m_end_indices,
                         [&](std::size_t idx) { return lhs[idx] == rhs[idx]; });
    }
    bool operator()(attribute_list const &lhs,
                    dataframe_tracker::record const &rhs) const
    {
      return operator()(rhs, lhs);
    }
  };
  template <class Iterator>
  static list_equaler<Iterator> make_list_equaler(Iterator first, Iterator last)
  {
    return list_equaler<Iterator>(first, last);
  }

  using set_type =
      boost::intrusive::unordered_set<record,                                 //
                                      boost::intrusive::hash<node_hash>,      //
                                      boost::intrusive::equal<node_equal>,    //
                                      boost::intrusive::compare_hash<true>,   //
                                      boost::intrusive::power_2_buckets<true> //
                                      >;

  using bucket_type = set_type::bucket_type;

public:
  static const std::size_t hash_seed = 0;

  using iterator = set_type::iterator;
  using const_iterator = set_type::const_iterator;
  using insert_commit_data = set_type::insert_commit_data;

  using attribute_visitor = std::function<void(attribute_list const &)>;

  static constexpr std::size_t minimal_bucket_count = 1024;
  dataframe_tracker(std::size_t record_size, std::size_t key_size);
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
  std::size_t const m_key_size;

  std::size_t m_bucket_count;
  std::unique_ptr<bucket_type[]> m_buckets;

  set_type m_attribute_set;
};
END_NAMESPACE_CORE

#endif // DATAFRAME_TRACKER_HPP
