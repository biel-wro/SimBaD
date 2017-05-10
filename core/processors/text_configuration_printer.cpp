#include "text_configuration_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_list.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
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

void text_configuration_printer::write_header(const configuration_view &conf)
{
  attribute_descriptor::const_iterator it = conf.descriptor().begin(),
                                    end = conf.descriptor().end();

  if(end != it)
    ostream() << "\"" << it->attribute_name() << "\"";

  for(++it; end != it; ++it)
    ostream() << m_delimiter << "\"" << it->attribute_name() << "\"";
  ostream() << std::endl;
}

void text_configuration_printer::write_data(const configuration_view &conf)
{
  std::vector<std::size_t> indices;
  std::vector<std::string> names;
  std::tie(indices, names) = conf.descriptor().unpack_all();

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
}
void text_configuration_printer::write_footer(const configuration_view &conf) {}
END_NAMESPACE_CORE
