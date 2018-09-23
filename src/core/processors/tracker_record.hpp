#ifndef SIMBAD_DATAFRAME_TRACKER_RECORD_HPP
#define SIMBAD_DATAFRAME_TRACKER_RECORD_HPP

#include "interface/interface_fwd.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/dataframe.hpp"

#include <boost/intrusive/unordered_set_hook.hpp>

#include <cstddef>
#include <memory>

BEGIN_NAMESPACE_CORE

using tracker_hook_type = boost::intrusive::unordered_set_base_hook<
    boost::intrusive::store_hash<true>>;

class tracker_record : public tracker_hook_type
{
  tracker_record() = default;
  ~tracker_record() = default;

public:
  using raw_pointer = char *;
  using raw_const_pointer = char const *;

  using pointer = attribute *;
  using const_pointer = attribute const *;

  template <class RecordPtr>
  using attribute_ptr_type =
      std::conditional_t<std::is_const<std::remove_pointer_t<RecordPtr>>::value,
                         const_pointer, pointer>;
  template <class RecordPtr>
  using raw_ptr_type =
      std::conditional_t<std::is_const<std::remove_pointer_t<RecordPtr>>::value,
                         raw_const_pointer, raw_pointer>;

  using iterator = pointer;
  using const_iterator = const_pointer;

  static std::size_t
  attribute_offset(std::size_t header_size = sizeof(tracker_record),
                   std::size_t attribute_align = alignof(attribute));
  static std::size_t record_bytes(std::size_t num_records);

  template <class RecordPtr>
  static constexpr attribute_ptr_type<RecordPtr>
  s_attribute_ptr(RecordPtr record_ptr, std::size_t idx);

  template <class AttributeList, class RecordIdxIterator>
  static tracker_record *initialize(void *memory_ptr, AttributeList &&list,
                                    RecordIdxIterator beg_idx,
                                    RecordIdxIterator last_idx);
  static tracker_record *initialize(void *memory_ptr, std::size_t record_length,
                                    attribute const &key);
  static tracker_record *initialize(void *memory_ptr,
                                    std::size_t record_length);

  static void *deinitialize(tracker_record *ptr, std::size_t record_length);

  pointer get_ptr(std::size_t idx);
  const_pointer get_ptr(std::size_t idx) const;
  attribute &get(std::size_t idx);
  attribute const &get(std::size_t idx) const;

  template <class AttributeList, class IdxIterator>
  void set(std::size_t start_idx, AttributeList &&source, IdxIterator it,
           IdxIterator end);

  std::size_t key_hash(std::size_t key_size) const;
  bool key_equal(tracker_record const &other, std::size_t key_size) const;

  bool key_equal(attribute const &attr) const;
  template <class AttributeList, class IdxIterator>
  bool key_equal(AttributeList &&other, IdxIterator idx_beg,
                 IdxIterator idx_end) const;

  struct key_hasher
  {
    std::size_t operator()(tracker_record const &n) const;
    std::size_t operator()(attribute const &attr) const;
  };

  struct key_equaler
  {
    bool operator()(tracker_record const &lhs, tracker_record const &rhs) const;
    bool operator()(tracker_record const &record, attribute const &attr) const;
    bool operator()(attribute const &attr, tracker_record const &record) const;
    bool operator()(attribute const &lhs, attribute const &rhs) const;
  };
};

END_NAMESPACE_CORE

#endif // SIMBAD_DATAFRAME_TRACKER_RECORD_HPP
