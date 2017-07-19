#include "csv_reader.hpp"

#include <interface/property_tree.hpp>
#include <interface/attribute_description.hpp>
#include <interface/attribute_list.hpp>

BEGIN_NAMESPACE_CORE
csv_reader::csv_reader(std::istream *istream, const property_tree &pt)
    : csv_reader(istream, pt.get<std::string>("delimiter", ","))
{
}

csv_reader::csv_reader(std::istream *istream, std::string delimiter)
    : stream_reader(istream), m_delimiter(std::move(delimiter))
{
}

attribute_description csv_reader::read_header() {
  std::string line;
  std::getline( istream(), line);
  throw("not implemented");
}

attribute_list csv_reader::read_entry(attribute_description const &description)
{
std::string line;
  std::getline( istream(), line);
  throw("not implemented");
}

void csv_reader::read_footer(attribute_description const &description) {}
END_NAMESPACE_CORE