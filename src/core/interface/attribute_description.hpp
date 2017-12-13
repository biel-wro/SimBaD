#ifndef ATTRIBUTE_DESCRIPTION_HPP
#define ATTRIBUTE_DESCRIPTION_HPP
#include "interface/interface_fwd.hpp"

#include <initializer_list>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/optional/optional_fwd.hpp>

//#include "interface/property_tree.hpp"
#include "interface/attribute_descriptor.hpp"

BEGIN_NAMESPACE_CORE

#define SIMBAD_ATTRIBUTES_DESCRIPTION_SUPER                                    \
  boost::multi_index_container<                                                \
      attribute_descriptor,                                                    \
      boost::multi_index::indexed_by<                                          \
          boost::multi_index::ordered_unique<                                  \
              boost::multi_index::const_mem_fun<                               \
                  attribute_descriptor, std::size_t,                           \
                  &attribute_descriptor::attribute_idx>>,                      \
          boost::multi_index::ordered_unique<                                  \
              boost::multi_index::const_mem_fun<                               \
                  attribute_descriptor, std::string const &,                   \
                  &attribute_descriptor::attribute_name>>,                     \
          boost::multi_index::ordered_non_unique<                              \
              boost::multi_index::const_mem_fun<                               \
                  attribute_descriptor, ATTRIBUTE_KIND,                        \
                  &attribute_descriptor::kind>>>>

class attribute_description : public SIMBAD_ATTRIBUTES_DESCRIPTION_SUPER
{
public:
  // types
  using super = SIMBAD_ATTRIBUTES_DESCRIPTION_SUPER;
  using index_iterator = super::nth_index<0>::type::iterator;
  using name_iterator = super::nth_index<1>::type::iterator;
  using kind_iterator = super::nth_index<2>::type::iterator;
  // constructors
  attribute_description();
  attribute_description(std::initializer_list<attribute_descriptor> list);

  // other constructions
  static attribute_description mapped_from(attribute_description const &other);
  static attribute_description
  mapped_from(attribute_description const &other,
              std::vector<std::string> const &names);

  template <class NameIterator>
  static attribute_description mapped_from(attribute_description const &other,
                                           NameIterator first,
                                           NameIterator last);

  // iterating
  index_iterator begin_indices() const;
  index_iterator end_indices() const;
  name_iterator begin_names() const;
  name_iterator end_names() const;
  kind_iterator begin_kinds() const;
  kind_iterator end_kinds() const;

  bool is_uniquely_defined(ATTRIBUTE_KIND kind, bool is_optional = true) const;
  // finding
  boost::optional<attribute_descriptor const &>
  get_descriptor(std::string const &name) const;
  boost::optional<attribute_descriptor const &>
  get_descriptor(std::size_t index) const;
  attribute_descriptor const &operator[](std::string const &name) const;
  boost::optional<attribute_descriptor const &>
  get_descriptor(ATTRIBUTE_KIND kind) const;

  std::size_t next_unused_idx(std::size_t start = 0) const;

  // adding attributes
  void add_attribute(std::size_t idx, std::string name,
                     ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INFO,
                     ATTRIBUTE_SCALAR scalar = ATTRIBUTE_SCALAR::UNKNOWN,
                     std::size_t dimension = 0);
  void add_attribute(attribute_descriptor &&record);
  void add_attribute(attribute_descriptor const &record);
  std::size_t
  add_attribute_auto_idx(std::size_t start_index, std::string name,
                         ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INFO,
                         ATTRIBUTE_SCALAR scalar = ATTRIBUTE_SCALAR::UNKNOWN,
                         std::size_t dimension = 0);
  std::size_t
  add_attribute_auto_idx(std::string name,
                         ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INFO,
                         ATTRIBUTE_SCALAR scalar = ATTRIBUTE_SCALAR::UNKNOWN,
                         std::size_t dimension = 0);

  std::size_t copy_attribute_auto_idx(attribute_descriptor const &source,
                                      std::size_t start_idx = 0);

  // returned indices might be useful sometimes
  std::unordered_map<std::size_t, std::string>
  add_and_map_attributes(property_tree const &pt, bool ignore_empty = true);

  // optional map new_idx=>old_idx might be useful sometimes
  std::unordered_map<std::size_t, std::size_t> add_and_map_attributes(
      attribute_description const &other, std::size_t start_target_idx = 0,
      std::unordered_set<std::string> const *names = nullptr);

  std::unordered_map<std::size_t, std::size_t>
  add_and_map_attributes(attribute_description const &other,
                         std::vector<std::string> const &names,
                         std::size_t start_target_idx = 0);

  void add_attributes(attribute_description const &other,
                      std::vector<std::string> const &names);
  // mappings
  std::vector<std::size_t>
  lin_mapping_from(attribute_description const &other) const;

  // unpacking
  std::pair<std::vector<std::size_t>, std::vector<std::string>>
  unpack_all() const;
  std::vector<std::size_t> unpack_indices() const;
  std::vector<std::size_t>
  names_to_indices(std::vector<std::string> const &names) const;

  void standardize_record(ATTRIBUTE_KIND attribute_kind,
                          std::string const &name);

  // some trivial descriptions
  static attribute_description const &make_empty();
  static attribute_description const &make_position_only();

private:
  template <class NameIterator>
  void add_attributes(attribute_description const &other, NameIterator first,
                      NameIterator last, std::size_t start_target_idx = 0);
  template <class Iterator>
  std::unordered_map<std::size_t, std::size_t>
  add_and_map_attributes(attribute_description const &other, Iterator first,
                         Iterator last, std::size_t start_target_idx);
};

std::ostream &operator<<(std::ostream &os, attribute_description const &desc);

#undef SIMBAD_ATTRIBUTES_DESCRIPTION_SUPER
END_NAMESPACE_CORE
#endif
