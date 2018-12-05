#include "attribute_description.hpp"
#include "attribute_descriptor.hpp"

#include "property_tree.hpp"
#include "utils/attribute_exceptions.hpp"

#include "interface/templates/attribute_description_templ.hpp"

#include <boost/optional.hpp>

#include <iterator>
#include <type_traits>

BEGIN_NAMESPACE_CORE
attribute_description::attribute_description() = default;
attribute_description::attribute_description(
    std::initializer_list<attribute_descriptor> list)
    : attribute_description()
{
  for(attribute_descriptor const &record : list)
    add_attribute(record);
}

attribute_description
attribute_description::mapped_from(attribute_description const &other)
{
  std::vector<std::string> names;
  names.reserve(other.size());
  for(attribute_descriptor const &descriptor : other)
    names.emplace_back(descriptor.attribute_name());
  return mapped_from(other, names);
}

attribute_description
attribute_description::mapped_from(attribute_description const &other,
                                   std::vector<std::string> const &names)
{
  return mapped_from(other, names.begin(), names.end());
}
attribute_description::index_iterator
attribute_description::begin_indices() const
{
  return get<0>().begin();
}

attribute_description::index_iterator attribute_description::end_indices() const
{
  return get<0>().end();
}

attribute_description::name_iterator attribute_description::begin_names() const
{
  return get<1>().begin();
}

attribute_description::name_iterator attribute_description::end_names() const
{
  return get<1>().end();
}

attribute_description::kind_iterator attribute_description::begin_kinds() const
{
  return get<2>().begin();
}

attribute_description::kind_iterator attribute_description::end_kinds() const
{
  return get<2>().end();
}

bool attribute_description::is_uniquely_defined(ATTRIBUTE_KIND kind,
                                                bool is_optional) const
{
  kind_iterator it = get<2>().find(kind);
  if(it == end_kinds())
    return is_optional;

  attribute_description::kind_iterator next = std::next(it);
  return next == end_kinds() || next->kind() != kind;
}

boost::optional<const attribute_descriptor &>
attribute_description::get_descriptor(const std::string &name) const
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    return boost::none;
  return *it;
}

boost::optional<const attribute_descriptor &>
attribute_description::get_descriptor(std::size_t index) const
{
  index_iterator it = get<0>().find(index);
  if(end_indices() == it)
    return boost::none;
  return *it;
}

const attribute_descriptor &attribute_description::
operator[](const std::string &name) const
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    throw unrecognized_attribute_name(name);
  return *it;
}

boost::optional<const attribute_descriptor &>
attribute_description::get_descriptor(ATTRIBUTE_KIND kind,
                                      bool try_default_name_too) const
{
  assert(ATTRIBUTE_KIND::PARTICLE_POSITION != kind ||
         is_uniquely_defined(kind));
  assert(ATTRIBUTE_KIND::PARTICLE_UID != kind || is_uniquely_defined(kind));
  assert(ATTRIBUTE_KIND::EVENT_UID != kind || is_uniquely_defined(kind));

  kind_iterator it = get<2>().find(kind);
  if(end_kinds() != it)
    return *it;

  if(try_default_name_too)
  {
    std::string standard_name = attribute_descriptor::default_name(kind);
    name_iterator name_it = get<1>().find(standard_name);
    if(end_names() != name_it)
      return *name_it;
  }

  return boost::none;
}

std::size_t attribute_description::next_unused_idx(std::size_t start) const
{
  index_iterator it = get<0>().find(start), end = end_indices();

  std::size_t val = start;
  for(; it != end; ++it)
  {
    if(val != it->attribute_idx())
      return val;
    ++val;
  }
  return val;
}

void attribute_description::add_attribute(std::size_t idx, std::string name,
                                          ATTRIBUTE_KIND kind,
                                          ATTRIBUTE_SCALAR scalar,
                                          std::size_t dimension)
{
  assert(get<0>().find(idx) == end_indices());

#if BOOST_VERSION >= 105500
  emplace(idx, std::move(name), kind, scalar, dimension);
#else
  insert(attribute_descriptor(idx, std::move(name), kind, scalar, dimension));
#endif
}

void attribute_description::add_attribute(attribute_descriptor &&record)
{
#if BOOST_VERSION >= 105500
  emplace(std::move(record));
#else
  insert(std::move(record));
#endif
}

void attribute_description::add_attribute(const attribute_descriptor &record)
{
  insert(record);
}

std::size_t attribute_description::add_attribute_auto_idx(
    std::size_t start_index, std::string name, ATTRIBUTE_KIND kind,
    ATTRIBUTE_SCALAR scalar, std::size_t dimension)
{
  std::size_t idx = next_unused_idx(start_index);
  add_attribute(idx, std::move(name), kind, scalar, dimension);
  return idx;
}

std::size_t attribute_description::add_attribute_auto_idx(
    std::string name, ATTRIBUTE_KIND kind, ATTRIBUTE_SCALAR scalar,
    std::size_t dimension)
{
  return add_attribute_auto_idx(0, std::move(name), kind, scalar, dimension);
}

std::size_t attribute_description::copy_attribute_auto_idx(
    const attribute_descriptor &source, std::size_t start_idx)
{
  return add_attribute_auto_idx(start_idx,                   //
                                source.attribute_name(),     //
                                source.kind(),               //
                                source.scalar(),             //
                                source.attribute_dimension() //
  );
}

std::pair<std::vector<std::size_t>, std::vector<std::string>>
attribute_description::unpack_all() const
{
  std::pair<std::vector<std::size_t>, std::vector<std::string>> ret;
  ret.first.reserve(size());
  ret.second.reserve(size());
  for(attribute_descriptor const &desc : *this)
  {
    ret.first.push_back(desc.attribute_idx());
    ret.second.push_back(desc.attribute_name());
  }
  return ret;
}

std::vector<std::size_t> attribute_description::unpack_indices() const
{
  std::vector<std::size_t> indices;
  indices.reserve(size());
  for(attribute_descriptor const &desc : *this)
    indices.push_back(desc.attribute_idx());
  return indices;
}

std::unordered_map<std::size_t, std::size_t>
attribute_description::reverse_mapping(std::vector<std::size_t> const &mapping)
{
  std::unordered_map<std::size_t, std::size_t> reversed;
  reversed.reserve(mapping.size());

  for(std::size_t from_idx = 0; from_idx != mapping.size(); ++from_idx)
  {
    std::size_t to_idx = mapping[from_idx];
    reversed[to_idx] = from_idx;
  }
  return reversed;
}

std::vector<std::size_t> attribute_description::names_to_indices(
    std::vector<std::string> const &names) const
{
  std::vector<std::size_t> result;
  result.reserve(names.size());

  for(std::string const &name : names)
  {
    boost::optional<const attribute_descriptor &> descriptor =
        get_descriptor(name);
    if(!descriptor)
      throw unrecognized_attribute_name(name);
    result.push_back(descriptor->attribute_idx());
  }

  return result;
}

bool attribute_description::standardize_all()
{
  std::vector<std::string> names;
  names.reserve(this->size());
  for(attribute_descriptor const &descriptor : *this)
    names.push_back(descriptor.attribute_name());

  bool ok = true;
  for(std::string const &name : names)
    ok = ok && standardize_record(name);
  return ok;
}
bool attribute_description::standardize_record(std::string const &name)
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    return false;

  return get<1>().modify(
      it, [](attribute_descriptor &desc) { desc.set_default_by_name(false); });
}
bool attribute_description::standardize_record(ATTRIBUTE_KIND kind)
{
  kind_iterator it = get<2>().find(kind);
  if(end_kinds() == it)
    return false;

  return get<2>().modify(
      it, [](attribute_descriptor &desc) { desc.set_default_by_kind(false); });
}

std::unordered_map<std::size_t, std::string>
attribute_description::add_and_map_attributes(const property_tree &pt,
                                              bool ignore_empty)
{
  std::unordered_map<std::size_t, std::string> values;

  pt.visit(
      [&](std::string const &name, std::string const &value) {
        std::size_t idx = add_attribute_auto_idx(
            0, name, ATTRIBUTE_KIND::INTRINSIC, ATTRIBUTE_SCALAR::UNKNOWN, 1);
        values.emplace(idx, value);
      },
      ignore_empty);

  return values;
}

std::unordered_map<std::size_t, std::size_t>
attribute_description::add_and_map_attributes(
    attribute_description const &other, std::size_t start_target_idx,
    std::unordered_set<std::string> const *names)
{
  std::unordered_map<std::size_t, std::size_t> new2old;

  std::size_t tgt_index = next_unused_idx(start_target_idx);

  name_iterator it = other.begin_names(), end = other.end_names();
  for(; it != end; ++it)
  {
    std::string const &name = it->attribute_name();

    if(names != nullptr && 0 == names->count(name))
      continue;

    std::size_t src_index = it->attribute_idx();
    new2old.emplace(tgt_index, src_index);

    insert(attribute_descriptor(tgt_index, name, it->kind(), it->scalar(),
                                it->attribute_dimension()));

    tgt_index = next_unused_idx(tgt_index + 1);
  }
  return new2old;
}

std::unordered_map<std::size_t, std::size_t>
attribute_description::add_and_map_attributes(
    attribute_description const &other, std::vector<std::string> const &names,
    std::size_t start_target_idx)
{
  return add_and_map_attributes(other, names.begin(), names.end(),
                                start_target_idx);
}

void attribute_description::add_attributes(
    attribute_description const &other, std::vector<std::string> const &names)
{
  add_attributes(other, names.begin(), names.end());
}

std::vector<std::size_t> attribute_description::lin_mapping_from(
    const attribute_description &other) const
{
  std::vector<std::size_t> mapping(this->size());

  for(attribute_descriptor const &descriptor : *this)
  {
    std::string const &name = descriptor.attribute_name();
    boost::optional<attribute_descriptor const &> other_descriptor =
        other.get_descriptor(name);
    if(!other_descriptor)
      throw unrecognized_attribute_name(name);
    std::size_t my_index = descriptor.attribute_idx();
    std::size_t other_index = other_descriptor->attribute_idx();

    mapping.at(my_index) = other_index;
  }
  return mapping;
}

const attribute_description &attribute_description::make_empty()
{
  static std::unique_ptr<attribute_description> mapping_ptr(
      new attribute_description);
  return *mapping_ptr;
}

const attribute_description &attribute_description::make_position_only()
{
  static std::unique_ptr<attribute_description> mapping_ptr;
  if(nullptr == mapping_ptr)
  {
    mapping_ptr.reset(new attribute_description);
    mapping_ptr->add_attribute(0, "position",
                               ATTRIBUTE_KIND::PARTICLE_POSITION);
  }
  return *mapping_ptr;
}

std::size_t attribute_description::get_attribute_idx(
    std::string const &attribute_name) const
{
  boost::optional<attribute_descriptor const &> maybe_descriptor =
      get_descriptor(attribute_name);
  if(!maybe_descriptor)
    throw std::logic_error("index for attribute `" + attribute_name +
                           "` not found");

  return maybe_descriptor->attribute_idx();
}

std::size_t
attribute_description::get_attribute_idx(ATTRIBUTE_KIND kind,
                                         bool try_default_name_too) const
{
  boost::optional<attribute_descriptor const &> maybe_descriptor =
      get_descriptor(kind, try_default_name_too);

  if(!maybe_descriptor)
    throw std::logic_error("index for attribute kind `" + to_string(kind) +
                           "` not found");

  return maybe_descriptor->attribute_idx();
}

std::ostream &operator<<(std::ostream &os, attribute_description const &desc)
{
  for(attribute_descriptor const &descriptor : desc.get<0>())
    os << descriptor << std::endl;
  return os;
}

END_NAMESPACE_CORE
