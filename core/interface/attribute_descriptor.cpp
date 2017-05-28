#include "attribute_descriptor.hpp"
#include "property_tree.hpp"
#include "utils/attribute_exceptions.hpp"

#include <boost/optional.hpp>
#include <boost/version.hpp>

BEGIN_NAMESPACE_CORE
attribute_descriptor::attribute_descriptor() {}
attribute_descriptor::attribute_descriptor(
    std::initializer_list<attribute_descriptor_record> list)
    : attribute_descriptor()
{
  for(attribute_descriptor_record const &record : list)
    add_attribute(record);
}
attribute_descriptor::index_iterator attribute_descriptor::begin_indices() const
{
  return get<0>().begin();
}

attribute_descriptor::index_iterator attribute_descriptor::end_indices() const
{
  return get<0>().end();
}

attribute_descriptor::name_iterator attribute_descriptor::begin_names() const
{
  return get<1>().begin();
}

attribute_descriptor::name_iterator attribute_descriptor::end_names() const
{
  return get<1>().end();
}

attribute_descriptor::kind_iterator attribute_descriptor::begin_kinds() const
{
  return get<2>().begin();
}

attribute_descriptor::kind_iterator attribute_descriptor::end_kinds() const
{
  return get<2>().end();
}

bool attribute_descriptor::is_uniquely_defined(ATTRIBUTE_KIND kind,
                                               bool is_optional) const
{
  kind_iterator it = get<2>().find(kind);
  if(it == end_kinds())
    return is_optional;

  attribute_descriptor::kind_iterator next = std::next(it);
  return next == end_kinds() || next->kind() != kind;
}

boost::optional<const attribute_descriptor_record &>
attribute_descriptor::get_descriptor(const std::string &name) const
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    return boost::none;
  return *it;
}

boost::optional<const attribute_descriptor_record &>
attribute_descriptor::get_descriptor(std::size_t index) const
{
  index_iterator it = get<0>().find(index);
  if(end_indices() == it)
    return boost::none;
  return *it;
}

const attribute_descriptor_record &attribute_descriptor::
operator[](const std::string &name) const
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    throw unrecognized_attribute_name(name);
  return *it;
}

boost::optional<const attribute_descriptor_record &>
attribute_descriptor::get_descriptor(ATTRIBUTE_KIND kind) const
{
  assert(ATTRIBUTE_KIND::POSITION != kind || is_uniquely_defined(kind));
  assert(ATTRIBUTE_KIND::PARTICLE_UID != kind || is_uniquely_defined(kind));
  assert(ATTRIBUTE_KIND::EVENT_UID != kind || is_uniquely_defined(kind));
  kind_iterator it = get<2>().find(kind);
  if(end_kinds() == it)
    return boost::none;

  return *it;
}

std::size_t attribute_descriptor::next_unused_idx(std::size_t start) const
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

void attribute_descriptor::add_attribute(std::size_t idx, std::string name,
                                         ATTRIBUTE_KIND kind,
                                         std::size_t dimension)
{
  assert(get<0>().find(idx) == end_indices());

#if BOOST_VERSION >= 105500
  emplace(idx, std::move(name), kind, dimension);
#else
  insert(attribute_descriptor_record(idx, std::move(name), kind, dimension));
#endif
}

void attribute_descriptor::add_attribute(attribute_descriptor_record &&record)
{
  emplace(std::move(record));
}

void attribute_descriptor::add_attribute(
    const attribute_descriptor_record &record)
{
  insert(record);
}

std::size_t attribute_descriptor::add_attribute_auto_idx(
    std::size_t start_index, std::string name, ATTRIBUTE_KIND kind,
    std::size_t dimension)
{
  std::size_t idx = next_unused_idx(start_index);
  add_attribute(idx, std::move(name), kind);
  return idx;
}

std::size_t attribute_descriptor::add_attribute_auto_idx(std::string name,
                                                         ATTRIBUTE_KIND kind,
                                                         std::size_t dimension)
{
  return add_attribute_auto_idx(0, std::move(name), std::move(kind),
                                std::move(dimension));
}

std::pair<std::vector<std::size_t>, std::vector<std::string>>
attribute_descriptor::unpack_all() const
{
  std::pair<std::vector<std::size_t>, std::vector<std::string>> ret;
  ret.first.reserve(size());
  ret.second.reserve(size());
  for(attribute_descriptor_record const &desc : *this)
  {
    ret.first.push_back(desc.attribute_idx());
    ret.second.push_back(desc.attribute_name());
  }
  return ret;
}

std::vector<std::size_t> attribute_descriptor::unpack_indices() const
{
  std::vector<std::size_t> indices;
  indices.reserve(size());
  for(attribute_descriptor_record const &desc : *this)
    indices.push_back(desc.attribute_idx());
  return indices;
}

std::unordered_map<std::size_t, std::string>
attribute_descriptor::add_attributes(const property_tree &pt, bool ignore_empty)
{
  std::unordered_map<std::size_t, std::string> values;

  pt.visit(
      [&](std::string const &name, std::string const &value) {
        std::size_t idx =
            add_attribute_auto_idx(0, name, ATTRIBUTE_KIND::INTRINSIC, 1);
        values.emplace(idx, value);
      },
      ignore_empty);

  return values;
}

std::unordered_map<std::size_t, std::size_t>
attribute_descriptor::add_attributes(
    const attribute_descriptor &other, std::size_t start_target_idx,
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
    insert(attribute_descriptor_record(tgt_index, name, kind));

    tgt_index = next_unused_idx(tgt_index + 1);
  }
  return new2old;
}

const attribute_descriptor &attribute_descriptor::make_empty()
{
  static std::unique_ptr<attribute_descriptor> mapping_ptr(
      new attribute_descriptor);
  return *mapping_ptr;
}

const attribute_descriptor &attribute_descriptor::make_position_only()
{
  static std::unique_ptr<attribute_descriptor> mapping_ptr;
  if(nullptr == mapping_ptr)
  {
    mapping_ptr.reset(new attribute_descriptor);
    mapping_ptr->add_attribute(0, "position", ATTRIBUTE_KIND::POSITION);
  }
  return *mapping_ptr;
}
END_NAMESPACE_CORE
