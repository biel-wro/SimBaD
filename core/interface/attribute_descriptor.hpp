#ifndef ATTRIBUTE_DESCRIPTOR_HPP
#define ATTRIBUTE_DESCRIPTOR_HPP
#include "core_fwd.hpp"
#include "utils/attribute_exceptions.hpp"

#include <boost/range/any_range.hpp>

#include <cstddef>
#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
enum class ATTRIBUTE_KIND : std::uint32_t
{
  INTRINSIC,   // attributes stored in particle, non-recomputable
  ACCUMULATED, // attributes stored in particle, recomputable
  OBSERVABLE,  // attributes not-stored in particle, with physical meaning
  INFO         // other attributes
};

class attribute_descriptor
{
public:
  explicit attribute_descriptor(std::string name, std::size_t attr_id,
                                ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INFO);
  std::string const &attribute_name() const;
  void set_attribute_name(std::string const &attribute_name);
  std::size_t attribute_idx() const;
  void set_attribute_id(std::size_t attribute_idx);
  ATTRIBUTE_KIND attribute_kind() const;
  void set_attribute_kind(ATTRIBUTE_KIND attribute_kind);

private:
  std::string m_attribute_name;
  std::size_t m_attribute_id;
  ATTRIBUTE_KIND m_attribute_kind;
};

class attribute_range : public boost::any_range<attribute_descriptor,
                                                boost::forward_traversal_tag,
                                                attribute_descriptor const &>
{
public:
  using super =
      boost::any_range<attribute_descriptor, boost::forward_traversal_tag,
                       attribute_descriptor const &>;
  using super::super;
};

template <class DescIt>
std::vector<std::size_t> used_attribute_indexes(DescIt beg, DescIt end)
{
  std::vector<std::size_t> indexes;
  std::transform(
      beg, end, std::back_inserter(indexes),
      [](attribute_descriptor const &desc) { return desc.attribute_idx(); });
  return indexes;
}

template <class DescIt>
std::vector<std::size_t> unused_attribute_indexes(DescIt beg, DescIt end,
                                                  std::size_t cnt,
                                                  std::size_t start_idx = 0)
{
  std::vector<std::size_t> unused(cnt);
  std::vector<std::size_t> used;

  for(DescIt desc_it = beg; desc_it != end; ++desc_it)
    used.push_back(desc_it->attribute_idx());

  std::greater<std::size_t> cmp;
  std::make_heap(used.begin(), used.end(), cmp);

  std::size_t idx = start_idx;
  for(std::size_t &new_idx : unused)
  {
    while(!used.empty() && idx >= used.front())
    {
      if(!used.empty())
      {
        std::pop_heap(used.begin(), used.end(), cmp);
        used.pop_back();
      }
      else
        ++idx;
    }
    new_idx = idx;
    ++idx;
  }
  return unused;
}

template <class DescriptorIterator, class NameIterator>
std::vector<std::size_t>
get_attribute_indexes_by_name(DescriptorIterator desc_beg,
                              DescriptorIterator desc_end,
                              NameIterator name_beg, NameIterator name_end)
{
  std::vector<std::size_t> indexes;
  for(NameIterator name_it = name_beg; name_it != name_end; ++name_it)
  {
    bool found = false;
    for(DescriptorIterator desc_it = desc_beg; desc_it != desc_end; ++desc_it)
    {
      if(*name_it == desc_it->attribute_name())
      {
        indexes.push_back(desc_it->attribute_idx());
        found = true;
        break;
      }
    }
    if(!found)
      throw unrecognized_attribute_name(*name_it);
  }
  return indexes;
}

END_NAMESPACE_CORE
#endif // ATTRIBUTE_DESCRIPTOR_HPP
