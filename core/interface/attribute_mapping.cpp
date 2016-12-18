#include "attribute_mapping.hpp"
#include "utils/attribute_exceptions.hpp"
BEGIN_NAMESPACE_CORE
attribute_mapping::attribute_mapping() {}
attribute_mapping::index_iterator attribute_mapping::begin_indices() const
{
  return get<0>().begin();
}

attribute_mapping::index_iterator attribute_mapping::end_indices() const
{
  return get<0>().end();
}

attribute_mapping::name_iterator attribute_mapping::begin_names() const
{
  return get<1>().begin();
}

attribute_mapping::name_iterator attribute_mapping::end_names() const
{
  return get<1>().end();
}

attribute_mapping::kind_iterator attribute_mapping::begin_kinds() const
{
  return get<2>().begin();
}

attribute_mapping::kind_iterator attribute_mapping::end_kinds() const
{
  return get<2>().end();
}

boost::optional<const attribute_descriptor &>
attribute_mapping::get_descriptor(const std::string &name) const
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    return boost::none;
  return *it;
}

boost::optional<const attribute_descriptor &>
attribute_mapping::get_descriptor(std::size_t index) const
{
  index_iterator it = get<0>().find(index);
  if(end_indices() == it)
    return boost::none;
  return *it;
}

const attribute_descriptor &attribute_mapping::
operator[](const std::string &name) const
{
  name_iterator it = get<1>().find(name);
  if(end_names() == it)
    throw unrecognized_attribute_name(name);
  return *it;
}

std::size_t attribute_mapping::next_unused_idx(std::size_t start) const
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

std::size_t attribute_mapping::add_attribute(std::string name,
                                             ATTRIBUTE_KIND kind,
                                             std::size_t start_index)
{
  std::size_t idx = next_unused_idx(start_index);
  emplace(idx, std::move(name), kind);
  return idx;
}

std::pair<std::vector<std::size_t>, std::vector<std::string>>
attribute_mapping::unpack_all() const
{
  std::pair<std::vector<std::size_t>, std::vector<std::string>> ret;
  ret.first.reserve(size());
  ret.second.reserve(size());
  for( attribute_descriptor const &desc : *this)
  {
    ret.first.push_back(desc.attribute_idx());
    ret.second.push_back(desc.attribute_name());
  }
  return ret;
}

std::unordered_map<std::size_t, std::string>
attribute_mapping::add_attributes(const property_tree &pt, bool ignore_empty)
{
  std::unordered_map<std::size_t, std::string> values;

  pt.visit(
      [&](std::string const &name, std::string const &value) {
        std::size_t idx = add_attribute(name, ATTRIBUTE_KIND::INTRINSIC);
        values.emplace(idx, value);
      },
      ignore_empty);

  return values;
}

std::unordered_map<std::size_t, std::size_t>
attribute_mapping::add_attributes(const attribute_mapping &other,
                                  std::size_t start_target_idx,
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
    ATTRIBUTE_KIND kind = it->attribute_kind();
    insert(attribute_descriptor(tgt_index, name, kind));

    tgt_index = next_unused_idx(tgt_index + 1);
  }
  return new2old;
}
END_NAMESPACE_CORE
