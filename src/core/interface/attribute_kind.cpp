#include "attribute_kind.hpp"

#include "utils/attribute_exceptions.hpp"

#include <boost/bimap.hpp>

#include <iostream>

BEGIN_NAMESPACE_CORE

using kind2str = boost::bimap<ATTRIBUTE_KIND, std::string>;

static std::unique_ptr<kind2str> make_mapping()
{
// clang-format off
#define SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(name) {ATTRIBUTE_KIND::name, #name }
  // clang-format on

  std::vector<kind2str::value_type> pairs{
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(POSITION),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(EVENT_UID),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(TIME),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(DELTA_TIME),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(EVENT_KIND),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(INTRINSIC),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(ACCUMULATED),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(OBSERVABLE),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(INFO),
      SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR(UNKNOWN),
  };
#undef SIMBAD_ATTRIBUTE_KIND_MAPPING_PAIR
  std::unique_ptr<kind2str> ptr(new kind2str(pairs.begin(), pairs.end()));
  return ptr;
}

static kind2str const &get_mapping()
{
  static std::unique_ptr<kind2str> ptr(make_mapping());
  return *ptr;
}

std::ostream &operator<<(std::ostream &os, ATTRIBUTE_KIND kind)
{
  kind2str const &mapping = get_mapping();
  kind2str::left_const_iterator it = mapping.left.find(kind);

  if(mapping.left.end() == it)
    throw unrecognized_attribute_kind(kind);

  return os << it->second;
}

std::istream &operator>>(std::istream &is, ATTRIBUTE_KIND &kind)
{
  std::string str;
  is >> str;

  kind2str const &mapping = get_mapping();
  kind2str::right_const_iterator it = mapping.right.find(str.c_str());

  if(mapping.right.end() == it)
    throw unrecognized_attribute_name(str);

  kind = it->second;
  return is;
}

END_NAMESPACE_CORE
