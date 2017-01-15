#ifndef ATTRIBUTE_DESCRIPTOR_HPP
#define ATTRIBUTE_DESCRIPTOR_HPP
#include "core_fwd.hpp"

//#include "interface/property_tree.hpp"
#include "interface/attribute_descriptor_record.hpp"

#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/optional/optional_fwd.hpp>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

BEGIN_NAMESPACE_CORE

#define SIMBAD_ATTRIBUTES_DESCRIPTOR_SUPER                                     \
  boost::multi_index_container<                                                \
      attribute_descriptor_record,                                             \
      boost::multi_index::indexed_by<                                          \
                                                                               \
          boost::multi_index::ordered_unique<                                  \
              boost::multi_index::const_mem_fun<                               \
                  attribute_descriptor_record, std::size_t,                    \
                  &attribute_descriptor_record::attribute_idx>>,               \
          boost::multi_index::ordered_unique<                                  \
              boost::multi_index::const_mem_fun<                               \
                  attribute_descriptor_record, std::string const &,            \
                  &attribute_descriptor_record::attribute_name>>,              \
          boost::multi_index::ordered_non_unique<                              \
              boost::multi_index::const_mem_fun<                               \
                  attribute_descriptor_record, ATTRIBUTE_KIND,                 \
                  &attribute_descriptor_record::kind>>>>

class attribute_descriptor : public SIMBAD_ATTRIBUTES_DESCRIPTOR_SUPER
{
public:
  // types
  using super = SIMBAD_ATTRIBUTES_DESCRIPTOR_SUPER;
  using index_iterator = super::nth_index<0>::type::iterator;
  using name_iterator = super::nth_index<1>::type::iterator;
  using kind_iterator = super::nth_index<2>::type::iterator;
  // constructors
  attribute_descriptor();

  // iterating
  index_iterator begin_indices() const;
  index_iterator end_indices() const;
  name_iterator begin_names() const;
  name_iterator end_names() const;
  kind_iterator begin_kinds() const;
  kind_iterator end_kinds() const;

  bool is_uniquely_defined(ATTRIBUTE_KIND kind, bool is_optional=true) const;
  // finding
  boost::optional<attribute_descriptor_record const &>
  get_descriptor(std::string const &name) const;
  boost::optional<attribute_descriptor_record const &>
  get_descriptor(std::size_t index) const;
  attribute_descriptor_record const &operator[](std::string const &name) const;
  boost::optional<attribute_descriptor_record const &>
  get_descriptor(ATTRIBUTE_KIND kind) const;

  std::size_t next_unused_idx(std::size_t start = 0) const;

  // adding attributes
  void add_attribute(std::size_t idx, std::string name,
                     ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INFO,
                     std::size_t dimension = 0);
  std::size_t add_attribute_auto_idx(std::size_t start_index, std::string name,
                                     ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INFO,
                                     std::size_t dimension = 0);

  // returned indices might be usefull sometimes
  std::unordered_map<std::size_t, std::string>
  add_attributes(property_tree const &pt, bool ignore_empty = true);

  // optional map new_idx=>old_idx might be usefull sometimes
  std::unordered_map<std::size_t, std::size_t>
  add_attributes(attribute_descriptor const &other,
                 std::size_t start_target_idx = 0,
                 std::unordered_set<std::string> const *names = nullptr);
  // unpacking
  std::pair<std::vector<std::size_t>, std::vector<std::string>>
  unpack_all() const;
  std::vector<std::size_t> unpack_indices() const;

  // some standard mappings
  static attribute_descriptor const &make_empty();
  static attribute_descriptor const &make_position_only();
};

#undef SIMBAD_ATTRIBUTES_DESCRIPTOR_SUPER
END_NAMESPACE_CORE
#endif
