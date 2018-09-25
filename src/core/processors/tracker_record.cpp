#include "tracker_record.hpp"

BEGIN_NAMESPACE_CORE

std::size_t tracker_record::attribute_offset(std::size_t header_size,
                                             std::size_t attribute_align)
{
  std::size_t excess_bytes = header_size % attribute_align;
  return 0 == excess_bytes ? header_size
                           : header_size + attribute_align - excess_bytes;
}

std::size_t tracker_record::record_bytes(std::size_t num_records)
{
  return attribute_offset() + num_records * sizeof(attribute);
}

template <class RecordPtr>
constexpr tracker_record::attribute_ptr_type<RecordPtr>
tracker_record::s_attribute_ptr(RecordPtr record_ptr, std::size_t idx)
{
  raw_ptr_type<RecordPtr> memory_ptr =
      reinterpret_cast<raw_ptr_type<RecordPtr>>(record_ptr);
  raw_ptr_type<RecordPtr> offset_ptr = memory_ptr + record_bytes(idx);

  attribute_ptr_type<RecordPtr> attribute_ptr =
      reinterpret_cast<attribute_ptr_type<RecordPtr>>(offset_ptr);
  return attribute_ptr;
}
tracker_record *tracker_record::initialize(void *memory_ptr,
                                           std::size_t record_length,
                                           attribute const &key)
{
  assert(record_length > 0);

  tracker_record *rec_ptr = new(memory_ptr) tracker_record;

  pointer beg = s_attribute_ptr(rec_ptr, 0),
          end = s_attribute_ptr(rec_ptr, record_length);

  new(beg) attribute(key);

  for(pointer attribute_ptr = beg + 1; attribute_ptr != end; ++attribute_ptr)
    new(attribute_ptr) attribute;

  return rec_ptr;
}

tracker_record *tracker_record::initialize(void *memory_ptr,
                                           std::size_t record_length)
{
  return initialize(memory_ptr, record_length, attribute{});
}

template <class AttributeList, class RecordIdxIterator>
tracker_record *
tracker_record::initialize(void *memory_ptr, AttributeList &&list,
                           RecordIdxIterator beg_idx, RecordIdxIterator end_idx)
{
  tracker_record *rec_ptr = new(memory_ptr) tracker_record;

  pointer attr_beg = s_attribute_ptr(rec_ptr, 0);

  RecordIdxIterator it_idx = beg_idx;
  pointer attr_ptr = attr_beg;

  for(; it_idx != end_idx; ++it_idx, ++attr_ptr)
  {
    assert(s_attribute_ptr(rec_ptr, static_cast<std::size_t>(std::distance(
                                        beg_idx, end_idx))) != attr_ptr);

    std::size_t source_idx = *it_idx;
    new(attr_ptr) attribute{list[source_idx]};
  }

  return rec_ptr;
}

void *tracker_record::deinitialize(tracker_record *tracker_ptr,
                                   std::size_t record_length)
{
  pointer beg = s_attribute_ptr(tracker_ptr, 0),
          end = s_attribute_ptr(tracker_ptr, record_length);

  for(pointer attr_ptr = beg; attr_ptr != end; ++attr_ptr)
    attr_ptr->~attribute();

  tracker_ptr->~tracker_record();

  return reinterpret_cast<void *>(tracker_ptr);
}

tracker_record::pointer tracker_record::get_ptr(std::size_t idx)
{
  return s_attribute_ptr(this, idx);
}
tracker_record::const_pointer tracker_record::get_ptr(std::size_t idx) const
{
  return s_attribute_ptr(this, idx);
}
attribute &tracker_record::get(std::size_t idx) { return *get_ptr(idx); }
attribute const &tracker_record::get(std::size_t idx) const
{
  return *get_ptr(idx);
}

template <class AttributeList, class IdxIterator>
void tracker_record::set(std::size_t start_idx, AttributeList &&source_list,
                         IdxIterator it, IdxIterator end)
{
  for(std::size_t target_idx = start_idx; it != end; ++it, ++target_idx)
  {
    attribute &target = get(target_idx);

    std::size_t source_idx = *it;
    attribute const &source = source_list[source_idx];

    target = source;
  }
}

std::size_t tracker_record::key_hash(std::size_t key_size) const
{
  assert(key_size > 0);

  if(1 == key_size)
    return std::hash<attribute>()(get(0));

  std::size_t seed = 0;
  for(std::size_t idx = 0; idx < key_size; ++idx)
  {
    attribute const &attr = this->get(idx);
    boost::hash_combine(seed, attr);
  }
  return seed;
}

bool tracker_record::key_equal(const simbad::core::tracker_record &other,
                               std::size_t key_size) const
{
  for(std::size_t idx = 0; idx < key_size; ++idx)
    if(get(idx) != other.get(idx))
      return false;
  return true;
}
bool tracker_record::key_equal(attribute const &attr) const
{
  return get(0) == attr;
}
template <class AttributeList, class IdxIterator>
bool tracker_record::key_equal(AttributeList &&other, IdxIterator it,
                               IdxIterator end) const
{
  for(std::size_t idx = 0; it != end; ++it)
  {
    std::size_t other_idx = *it;
    if(this->get(idx) != other[other_idx])
      return false;
  }
  return true;
}

std::size_t tracker_record::key_hasher::
operator()(tracker_record const &record) const
{
  return record.key_hash(1);
}

std::size_t tracker_record::key_hasher::operator()(attribute const &attr) const
{
  return attr.hash_value();
}

bool tracker_record::key_equaler::operator()(const tracker_record &lhs,
                                             const tracker_record &rhs) const
{
  return lhs.key_equal(rhs, 1);
}

bool tracker_record::key_equaler::operator()(tracker_record const &rec,
                                             attribute const &attr) const
{
  return rec.key_equal(attr);
}
bool tracker_record::key_equaler::operator()(attribute const &attr,
                                             tracker_record const &rec) const
{
  return rec.key_equal(attr);
}

bool tracker_record::key_equaler::operator()(const attribute &lhs,
                                             const attribute &rhs) const
{
  return lhs == rhs;
}

#define TRACKER_RECORD_EXPLICIT_INSTANTIATION_ATTR(AttrList, Iter)             \
  template tracker_record *tracker_record::initialize<AttrList, Iter>(         \
      void *, AttrList, Iter, Iter);                                           \
  template void tracker_record::set<AttrList, Iter>(std::size_t, AttrList,     \
                                                    Iter, Iter);               \
  template bool tracker_record::key_equal<AttrList, Iter>(AttrList, Iter,      \
                                                          Iter) const;

#define TRACKER_RECORD_EXPLICIT_INSTANTIATION(Iter)                            \
  TRACKER_RECORD_EXPLICIT_INSTANTIATION_ATTR(attribute_list const &, Iter);    \
  TRACKER_RECORD_EXPLICIT_INSTANTIATION_ATTR(attribute_list &, Iter);          \
  TRACKER_RECORD_EXPLICIT_INSTANTIATION_ATTR(attribute_list &&, Iter);

TRACKER_RECORD_EXPLICIT_INSTANTIATION(std::vector<std::size_t>::iterator);
TRACKER_RECORD_EXPLICIT_INSTANTIATION(std::vector<std::size_t>::const_iterator);

END_NAMESPACE_CORE