#include "csv_reader.hpp"

#include "detail/csv_header_parser.hpp"
#include "detail/csv_record_parser.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/property_tree.hpp"

#include <boost/spirit/include/qi.hpp>

BEGIN_NAMESPACE_CORE
namespace
{
class attribute_list_view : public attribute_list
{
public:
  attribute get_attribute(std::size_t idx) const override
  {
    return m_attributes.at(idx);
  }
  std::vector<attribute> &attributes() { return m_attributes; }
  std::vector<attribute> const &attributes() const { return m_attributes; }
  ~attribute_list_view() {}

private:
  std::vector<attribute> m_attributes;
};
}
using iterator_type = std::string::const_iterator;

struct csv_reader::header_parser : public csv_header_parser<iterator_type>
{
  using csv_header_parser<iterator_type>::csv_header_parser;
};

struct csv_reader::record_parser : public csv_record_parser<iterator_type>
{
  using csv_record_parser<iterator_type>::csv_record_parser;
};

csv_reader::csv_reader(std::istream *istream, const property_tree &pt)
    : csv_reader(istream, pt.get<std::string>("delimiter", ","))
{
}

csv_reader::csv_reader(std::istream *istream, std::string const &delimiter,
                       std::string const &numsep)
    : stream_reader(istream),
      m_header_parser_ptr(new header_parser(delimiter, numsep)),
      m_record_parser_ptr(new record_parser(delimiter))
{
}

csv_reader::~csv_reader() {}

attribute_description csv_reader::read_header()
{
  std::string line_readout;
  std::getline(istream(), line_readout);

  iterator_type first = line_readout.begin(), last = line_readout.end();

  std::vector<std::pair<std::string, std::size_t>> attribute_names;

  bool ok = m_header_parser_ptr->parse(first, last, attribute_names);
  if(!ok)
    throw(std::runtime_error("could not parse csv header: " + line_readout));

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

void csv_reader::visit_entries(entry_visitor v, std::size_t max_reads)
{
  std::string line_buffer;
  attribute_list_view view;
  std::vector<attribute> &attribute_buffer(view.attributes());

  for(std::size_t i = 0; max_reads == 0 || i < max_reads; ++i)
  {
    std::getline(istream(), line_buffer);
    iterator_type first = line_buffer.begin(), last = line_buffer.end();

    if(first == last)
      break;

    attribute_buffer.clear();
    bool ok = m_record_parser_ptr->parse(first, last, m_attribute_sizes,
                                         attribute_buffer);
    if(!ok)
      throw(std::runtime_error("could not parse csv row: " + line_buffer));

    if(first != last)
      throw(std::runtime_error("extra tokens left at the end of row: " +
                               std::string(first, last)));
    v(view);
  }
}

END_NAMESPACE_CORE
