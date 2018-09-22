#include "tracker_record.hpp"

BEGIN_NAMESPACE_CORE

tracker_record::tracker_record(data_type_ptr ptr) : m_attributes_data_ptr(ptr)
{
  assert(nullptr != ptr);
}

template <class IdxIterator>
void tracker_record::set(attribute_list const &source, IdxIterator it,
                         IdxIterator end)
{
  for(std::size_t idx = 0; it != end; ++it)
  {
    std::size_t source_idx = *it;
    m_attributes_data_ptr[idx] = source[source_idx];
  }
}

attribute const &tracker_record::get(std::size_t idx) const
{
  return m_attributes_data_ptr[idx];
}

attribute &tracker_record::get(std::size_t idx)
{
  return m_attributes_data_ptr[idx];
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
    if(this->get(idx) != other.get(idx))
      return false;
  return true;
}
bool tracker_record::key_equal(attribute const &attr) const
{
  return m_attributes_data_ptr[0] == attr;
}
template <class IdxIterator>
bool tracker_record::key_equal(attribute_list const &other, IdxIterator it,
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

END_NAMESPACE_CORE