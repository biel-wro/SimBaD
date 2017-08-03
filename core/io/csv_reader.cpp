#include "csv_reader.hpp"

#include <interface/attribute_description.hpp>
#include <interface/attribute_descriptor.hpp>
#include <interface/attribute_list.hpp>
#include <interface/property_tree.hpp>

#include <boost/spirit/include/qi.hpp>

BEGIN_NAMESPACE_CORE

class csv_reader::attribute_readout : public attribute_list
{
public:
  attribute get_attribute(std::size_t idx) const override
  {
    return m_attributes.at(idx);
  }
  std::vector<attribute> &attributes() { return m_attributes; }
  std::vector<attribute> const &attributes() const { return m_attributes; }
  ~attribute_readout() {}

private:
  std::vector<attribute> m_attributes;
};

csv_reader::csv_reader(std::istream *istream, const property_tree &pt)
    : csv_reader(istream, pt.get<std::string>("delimiter", ","))
{
}

csv_reader::csv_reader(std::istream *istream, std::string const &delimiter,
                       std::string const &numsep)
    : stream_reader(istream),
      m_attribute_readout(new attribute_readout),
      m_header_parser(delimiter, numsep),
      m_record_parser(delimiter)
{
}

csv_reader::~csv_reader() {}

attribute_description csv_reader::read_header()
{
  std::getline(istream(), m_line_readout);

  iterator_type first = m_line_readout.begin(), last = m_line_readout.end();

  std::vector<std::pair<std::string, std::size_t>> attribute_names;

  bool ok = m_header_parser.parse(first, last, attribute_names);
  if(!ok)
    throw(std::runtime_error("could not parse csv header: " + m_line_readout));

  attribute_description description;
  m_attribute_sizes.clear();
  for(std::pair<std::string, std::size_t> const &pair : attribute_names)
  {
    m_attribute_sizes.push_back(pair.second);
    description.add_attribute_auto_idx(pair.first, ATTRIBUTE_KIND::INFO,
                                       ATTRIBUTE_SCALAR::UNKNOWN, pair.second);
  }
  return description;
}

attribute_list const &
csv_reader::read_entry(attribute_description const &description)
{
  std::getline(istream(), m_line_readout);
  iterator_type first = m_line_readout.begin(), last = m_line_readout.end();
  bool ok = m_record_parser.parse(first, last, m_attribute_sizes,
                                  m_attribute_readout->attributes());
  if(!ok)
    throw(std::runtime_error("could not parse csv row: " + m_line_readout));
  return *m_attribute_readout;
}

void csv_reader::read_footer(attribute_description const &description) {}
END_NAMESPACE_CORE
