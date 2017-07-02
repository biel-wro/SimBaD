#include "attribute_description.hpp"
#include "property_tree.hpp"
#include "utils/attribute_exceptions.hpp"

#include <boost/optional.hpp>
#include <boost/version.hpp>

BEGIN_NAMESPACE_CORE
attribute_description::attribute_description() {}
attribute_description::attribute_description(
    std::initializer_list<attribute_descriptor> list)
    : attribute_description()
{
  for(attribute_descriptor const &record : list)
    add_attribute(record);
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
attribute_description::get_descriptor(ATTRIBUTE_KIND kind) const
{
  assert(ATTRIBUTE_KIND::POSITION != kind || is_uniquely_defined(kind));
  assert(ATTRIBUTE_KIND::PARTICLE_UID != kind || is_uniquely_defined(kind));
  assert(ATTRIBUTE_KIND::EVENT_UID != kind || is_uniquely_defined(kind));
  kind_iterator it = get<2>().find(kind);
  if(end_kinds() == it)
    return boost::none;

  return *it;
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
  insert(attribute_descriptor(idx, std::move(name), kind, scalar,
                                     dimension));
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

std::unordered_map<std::size_t, std::string>
attribute_description::add_attributes(const property_tree &pt,
                                      bool ignore_empty)
{
  std::unordered_map<std::size_t, std::string> values;

  pt.visit(
      [&](std::string const &name, std::string const &value) {
        std::size_t idx =
            add_attribute_auto_idx(0, name, ATTRIBUTE_KIND::INTRINSIC,
                                   ATTRIBUTE_SCALAR ::UNKNOWN, 1);
        values.emplace(idx, value);
      },
      ignore_empty);

  return values;
}

std::unordered_map<std::size_t, std::size_t>
attribute_description::add_attributes(
    const attribute_description &other, std::size_t start_target_idx,
    const std::unordered_set<std::string> *names)
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
    ATTRIBUTE_KIND kind = it->kind();
    insert(attribute_descriptor(tgt_index, name, kind));

    tgt_index = next_unused_idx(tgt_index + 1);
  }
  return new2old;
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
    mapping_ptr->add_attribute(0, "position", ATTRIBUTE_KIND::POSITION);
  }
  return *mapping_ptr;
}
END_NAMESPACE_CORE
