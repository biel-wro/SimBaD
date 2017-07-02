#include "text_configuration_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"
#include <cstddef>
#include <iostream>

BEGIN_NAMESPACE_CORE
text_configuration_printer::text_configuration_printer()
    : stream_printer(nullptr)
{
}
text_configuration_printer::text_configuration_printer(std::ostream *ostream,
                                                       std::string delim)
    : stream_printer(*ostream), m_delimiter(std::move(delim))
{
}

text_configuration_printer::text_configuration_printer(std::ostream *ostream,
                                                       const property_tree &pt)
    : text_configuration_printer(ostream,
                                 pt.get<std::string>("delimiter", ", "))
{
}

void text_configuration_printer::write_header(attribute_description const &desc)
{
  std::tie(m_indices, m_names) = desc.unpack_all();

  attribute_description::const_iterator it = desc.begin(), end = desc.end();

  if(end != it)
    ostream() << "\"" << it->attribute_name() << "\"";

  for(++it; end != it; ++it)
    ostream() << m_delimiter << "\"" << it->attribute_name() << "\"";
  ostream() << std::endl;
}
/*
void text_configuration_printer::write_data(dataframe const &conf)
{
  std::vector<std::size_t>::const_iterator beg_idx = indices.begin(),
                                           end_idx = indices.end();
  std::vector<std::string>::const_iterator beg_names = names.begin();

  std::ostream &os = ostream();
  conf.visit_records([=, &os](attribute_list const &a) {

    if(end_idx != beg_idx)
      os << *beg_names << "=" << a[*beg_idx];

    std::vector<std::size_t>::const_iterator it_idx = std::next(beg_idx, 1);
    std::vector<std::string>::const_iterator it_names = std::next(beg_names, 1);
    for(; end_idx != it_idx; ++it_idx, ++it_names)
      os << m_delimiter << *it_names << "=" << a[*it_idx];
    os << std::endl;
  });
}*/

void text_configuration_printer::write_entry(attribute_list const &entry)
{
  std::vector<std::size_t>::const_iterator beg_idx = m_indices.begin(),
                                           end_idx = m_indices.end();
  std::vector<std::string>::const_iterator beg_names = m_names.begin();

  std::ostream &os = ostream();

  if(end_idx != beg_idx)
    os << *beg_names << "=" << entry[*beg_idx];

  std::vector<std::size_t>::const_iterator it_idx = std::next(beg_idx, 1);
  std::vector<std::string>::const_iterator it_names = std::next(beg_names, 1);
  for(; end_idx != it_idx; ++it_idx, ++it_names)
    os << m_delimiter << *it_names << "=" << entry[*it_idx];
  os << std::endl;
}
void text_configuration_printer::write_footer() {}
END_NAMESPACE_CORE
