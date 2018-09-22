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

struct tracker_record : public tracker_hook_type
{
  using data_type_ptr = attribute *;

  explicit tracker_record(data_type_ptr);

  attribute const &get(std::size_t idx) const;
  attribute &get(std::size_t idx);

  template <class IdxIterator>
  void set(attribute_list const &source, IdxIterator it, IdxIterator end);

  std::size_t key_hash(std::size_t key_size) const;
  bool key_equal(tracker_record const &other,
                 std::size_t key_size) const;

  bool key_equal(attribute const &attr) const;
  template <class IdxIterator>
  bool key_equal(attribute_list const &other, IdxIterator idx_beg,
                 IdxIterator idx_end) const;

  struct key_hasher
  {
    std::size_t operator()(tracker_record const &n) const;
    std::size_t operator()(attribute const &attr) const;
  };

  struct key_equaler
  {
    bool operator()(tracker_record const &lhs,
                    tracker_record const &rhs) const;
    bool operator()(tracker_record const &record,
                    attribute const &attr) const;
    bool operator()(attribute const &attr,
                    tracker_record const &record) const;
    bool operator()(attribute const &lhs, attribute const &rhs) const;
  };

private:
  data_type_ptr const m_attributes_data_ptr;
};


END_NAMESPACE_CORE

#endif // SIMBAD_DATAFRAME_TRACKER_RECORD_HPP
