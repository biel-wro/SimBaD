#include "csv_reader.hpp"

#include <interface/attribute_description.hpp>
#include <interface/attribute_descriptor.hpp>
#include <interface/attribute_list.hpp>
#include <interface/property_tree.hpp>

#include <boost/spirit/include/qi.hpp>

BEGIN_NAMESPACE_CORE

csv_reader::csv_reader(std::istream *istream, const property_tree &pt)
    : csv_reader(istream, pt.get<std::string>("delimiter", ","))
{
}

csv_reader::csv_reader(std::istream *istream, std::string const &delimiter,
                       std::string const &numsep)
    : stream_reader(istream), m_parser(delimiter, numsep)
{
}

attribute_description csv_reader::read_header()
{
  std::string line;
  std::getline(istream(), line);

  iterator_type first = line.begin(), last = line.end();

  bool ok = m_parser.parse(first, last, m_attribute_sizes);

  attribute_description description;
  for(std::pair<std::string, std::size_t> const &pair : m_attribute_sizes)
    description.add_attribute_auto_idx(pair.first, ATTRIBUTE_KIND::INFO,
                                       ATTRIBUTE_SCALAR::UNKNOWN, pair.second);

  return description;
}

attribute_list csv_reader::read_entry(attribute_description const &description)
{
  throw("not implemented");
}

void csv_reader::read_footer(attribute_description const &description) {}
END_NAMESPACE_CORE