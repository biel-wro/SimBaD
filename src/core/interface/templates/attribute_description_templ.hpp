#ifndef ATTRIBUTE_DESCRIPTION_TEMPL_HPP
#define ATTRIBUTE_DESCRIPTION_TEMPL_HPP
#include "interface/attribute_description.hpp"

#include "utils/attribute_exceptions.hpp"

#include <boost/optional.hpp>

BEGIN_NAMESPACE_CORE

template <class NameIterator>
attribute_description
attribute_description::mapped_from(attribute_description const &other,
                                   NameIterator first, NameIterator last)
{
  attribute_description desc;
  desc.add_attributes(other, first, last);
  return desc;
}

template <class NameIterator>
void attribute_description::add_attributes(attribute_description const &other,
                                           NameIterator first,
                                           NameIterator last,
                                           std::size_t start_target_idx)
{
  std::size_t current_idx = start_target_idx;
  for(NameIterator it = first; it != last; ++it)
  {
    std::string const &name = *it;
    boost::optional<attribute_descriptor const &> descriptor =
        other.get_descriptor(name);
    if(!descriptor)
      throw unrecognized_attribute_name(name);

    current_idx =
        1 + add_attribute_auto_idx(current_idx, name, descriptor->kind(),
                                   descriptor->scalar(),
                                   descriptor->attribute_dimension());
  }
}

template <class Iterator>
std::unordered_map<std::size_t, std::size_t>
attribute_description::add_and_map_attributes(
    const attribute_description &other, Iterator first, Iterator last,
    std::size_t start_target_idx)
{
  std::unordered_map<std::size_t, std::size_t> new_to_old;

  if(std::is_convertible<
         typename std::iterator_traits<Iterator>::iterator_category,
         std::random_access_iterator_tag>::value)
    new_to_old.reserve(std::distance(first, last));

  std::size_t tgt_index = next_unused_idx(start_target_idx);

  for(Iterator it = first; it < last; ++it)
  {
    std::string const &name = *it;

    boost::optional<attribute_descriptor const &> descriptor =
        other.get_descriptor(name);
    if(!descriptor)
      throw unrecognized_attribute_name(name);

    std::size_t src_index = descriptor->attribute_idx();
    new_to_old.emplace(tgt_index, src_index);
    ATTRIBUTE_KIND kind = descriptor->kind();
    insert(attribute_descriptor(tgt_index, name, kind));
    tgt_index = next_unused_idx(tgt_index + 1);
  }

  return new_to_old;
}



END_NAMESPACE_CORE
#endif // ATTRIBUTE_DESCRIPTION_TEMPL_HPP
