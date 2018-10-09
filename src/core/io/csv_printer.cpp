#include "csv_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"

#include <limits>
#include <iomanip>
#include <sstream>

BEGIN_NAMESPACE_CORE

constexpr char default_delimiter[] = ",";
constexpr std::size_t default_digits =
    std::numeric_limits<attribute::real_type>::max_digits10;

csv_printer::csv_printer(property_tree const &pt)
    : stream_printer(pt.get("file", "STDIN")),
      m_delimiter(pt.get("delimiter", default_delimiter)),
      m_num_digits(pt.get("digits", default_digits))
{
}
csv_printer::csv_printer(property_tree const &pt, std::ostream *ostream_ptr)
    : csv_printer(ostream_ptr, pt.get("digits", default_digits),
                  pt.get<std::string>("delimiter", default_delimiter))
{
}
csv_printer::csv_printer(std::ostream *ostream_ptr)
    : csv_printer(ostream_ptr, default_digits, default_delimiter)
{
}

csv_printer::csv_printer(std::ostream *ostream, std::size_t digits,
                         std::string delimiter)
    : stream_printer(ostream),
      m_delimiter(std::move(delimiter)),
      m_num_digits(digits)
{
}

static std::ostream &write_header_part(std::ostream &os,
                                       attribute_descriptor const &desc,
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

void csv_printer::write_header(attribute_description const &desc)
{
  m_dimensions.clear();
  m_dimensions.reserve(desc.size());
  m_indices = desc.unpack_indices();

  attribute_description::const_iterator it = desc.begin(), end = desc.end();

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

  std::stringstream buf;
  buf.precision(m_num_digits);

  if(end == beg)
    return (void)(buf << std::endl);

  s_write_data_part(buf, entry[*beg], m_dimensions[*beg], m_delimiter);

  std::vector<std::size_t>::const_iterator ind_it = std::next(beg, 1);
  std::vector<std::size_t>::const_iterator dim_it =
      std::next(m_dimensions.begin(), 1);

  for(; end != ind_it; ++ind_it, ++dim_it)
  {
    std::size_t attr_idx = *ind_it;
    attribute const &attr = entry[attr_idx];
    std::size_t dim = *dim_it;
    buf << m_delimiter;
    s_write_data_part(buf, attr, dim, m_delimiter);
  }
  buf << std::endl;

  ostream() << buf.str();
}

void csv_printer::write_data(dataframe const &conf)
{
  std::vector<std::size_t> indices = conf.description().unpack_indices();
  std::vector<std::size_t>::const_iterator beg = indices.begin(),
                                           end = indices.end();

  if(end == beg)
    return;

  std::ostream &os = ostream();

  conf.visit_records([this, beg, end, &os](attribute_list const &attr) {
    s_write_data_part(os, attr[*beg], m_dimensions[*beg], m_delimiter);

    for(std::vector<std::size_t>::const_iterator it = std::next(beg, 1);
        end != it; ++it)
      s_write_data_part(os << m_delimiter, attr[*it], m_dimensions[*it],
                        m_delimiter);

    os << std::endl;
  });
}
void csv_printer::write_footer() { ostream() << std::endl; }

END_NAMESPACE_CORE
