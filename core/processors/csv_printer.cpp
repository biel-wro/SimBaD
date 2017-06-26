#include "csv_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE

csv_printer::csv_printer(std::ostream *ostream, const property_tree &pt)
    : csv_printer(ostream, pt.get<std::string>("delimiter", ","))
{
}
csv_printer::csv_printer(std::ostream *ostream, std::string delimiter)
    : stream_printer(ostream), m_delimiter(std::move(delimiter))
{
}

static std::ostream &write_header_part(std::ostream &os,
                                       attribute_descriptor_record const &desc,
                                       std::vector<std::size_t> &dims,
                                       std::string const &delim)
{
  std::size_t dim = desc.attribute_dimension();
  std::string const &name = desc.attribute_name();

  dims.push_back(dim);
  if(0 == dim || 1 == dim)
    return os << "\"" << name << "\"";

  os << "\"" << name << "_0\"";

  for(std::size_t d = 1; d < dim; ++d)
    os << delim << "\"" << name << "_" << d << "\"";

  return os;
}

void csv_printer::write_header(const attribute_descriptor &desc)
{
  m_dimensions.clear();
  m_dimensions.reserve(desc.size());
  m_indices = desc.unpack_indices();

  attribute_descriptor::const_iterator it = desc.begin(), end = desc.end();

  if(end != it)
    write_header_part(ostream(), *it, m_dimensions, m_delimiter);
  for(++it; end != it; ++it)
    write_header_part(ostream() << m_delimiter, *it, m_dimensions, m_delimiter);

  ostream() << std::endl;
}

static std::ostream &s_write_data_part(std::ostream &os, attribute const &attr,
                                       std::size_t const &dim,
                                       std::string const &delim)
{
  if(0 == dim)
    return os << "\"" << attr << "\"";
  if(1 == dim)
    return os << attr;

  os << attr.get_scalar(0);
  for(std::size_t d = 1; d < dim; ++d)
    os << delim << attr.get_scalar(d);
  return os;
}

void csv_printer::write_entry(const attribute_list &entry)
{
  std::vector<std::size_t>::const_iterator beg = m_indices.begin(),
                                           end = m_indices.end();
  std::ostream &os = ostream();

  if(end != beg)
    s_write_data_part(os, entry[*beg], m_dimensions[*beg], m_delimiter);

  std::vector<std::size_t>::const_iterator it = std::next(beg, 1);
  for(; end != it; ++it)
    s_write_data_part(os << m_delimiter, entry[*it], m_dimensions[*it],
                      m_delimiter);

  os << std::endl;
}


void csv_printer::write_data(dataframe const &conf)
{
  std::vector<std::size_t> indices = conf.descriptor().unpack_indices();
  std::vector<std::size_t>::const_iterator beg = indices.begin(),
                                           end = indices.end();
  std::ostream &os = ostream();
  conf.visit_records([=, &os](attribute_list const &a) {
    if(end != beg)
      s_write_data_part(os, a[*beg], m_dimensions[*beg], m_delimiter);
    std::vector<std::size_t>::const_iterator it = std::next(beg, 1);
    for(; end != it; ++it)
      s_write_data_part(os << m_delimiter, a[*it], m_dimensions[*it],
                        m_delimiter);

    os << std::endl;
  });
}
void csv_printer::write_footer() { ostream() << std::endl; }
END_NAMESPACE_CORE
