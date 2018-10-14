#include "csv_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"

#include <iomanip>
#include <limits>
#include <sstream>

BEGIN_NAMESPACE_CORE

std::size_t const csv_printer::double_digits =
    std::numeric_limits<attribute::real_type>::max_digits10;
char constexpr csv_printer::default_delimiter[];

static csv_printer::precision_map
read_special_precisons(property_tree const &pt)
{
  csv_printer::precision_map result;
  property_tree const &special_pt =
      pt.get_child("special_precision", property_tree::get_empty());
  for(auto const &pair : special_pt)
  {
    std::string const &name = pair.first;
    std::size_t precison = pair.second.get_value<std::size_t>();
    result.emplace(std::make_pair(name, precison));
  }
  return result;
}

csv_printer::csv_printer(property_tree const &pt)
    : stream_printer(pt.get("file", "STDIN")),
      m_delimiter{pt.get("delimiter", default_delimiter)},
      m_default_precison_digits{pt.get("default_precision", double_digits)},
      m_special_precison(read_special_precisons(pt))
{
}
csv_printer::csv_printer(property_tree const &pt, std::ostream *ostream_ptr)
    : csv_printer(ostream_ptr,
                  pt.get<std::string>("delimiter", default_delimiter),
                  pt.get("digits", double_digits), read_special_precisons(pt))
{
}

csv_printer::csv_printer(std::ostream *ostream, std::string delimiter,
                         std::size_t default_digits,
                         precision_map special_precision_map)
    : stream_printer(ostream),
      m_delimiter(std::move(delimiter)),
      m_default_precison_digits(default_digits),
      m_special_precison(std::move(special_precision_map)),
      m_infos()
{
}

static std::ostream &s_write_header_part(std::ostream &os,
                                         attribute_descriptor const &desc,
                                         std::string const &delim)
{
  std::string const &name = desc.attribute_name();
  std::size_t dim = desc.attribute_dimension();

  if(0 == dim || 1 == dim)
    return os << "\"" << name << "\"";

  os << "\"" << name << "_0\"";

  for(std::size_t d = 1; d < dim; ++d)
    os << delim << "\"" << name << "_" << d << "\"";

  return os;
}

void csv_printer::write_header(attribute_description const &desc)
{
  m_infos.resize(desc.size());

  std::transform(desc.begin(), desc.end(), m_infos.begin(),
                 [this](attribute_descriptor const &desc) {
                   column_info result;
                   result.dimension = desc.attribute_dimension();
                   result.index = desc.attribute_idx();

                   std::string const &name = desc.attribute_name();

                   result.precision_digits = m_special_precison.count(name)
                                                 ? m_special_precison.at(name)
                                                 : m_default_precison_digits;

                   return result;
                 });

  attribute_description::const_iterator it = desc.begin(), end = desc.end();

  if(end != it)
    s_write_header_part(ostream(), *it, m_delimiter);
  for(++it; end != it; ++it)
    s_write_header_part(ostream() << m_delimiter, *it, m_delimiter);

  ostream() << std::endl;
}

std::ostream &csv_printer::s_write_data_part(std::ostream &os,
                                             attribute const &attr,
                                             column_info const &info,
                                             std::string const &delim)
{
  os.precision(info.precision_digits);

  if(0 == info.dimension)
    return os << "\"" << attr << "\"";
  if(1 == info.dimension)
    return os << attr;

  os << attr.get_scalar(0);
  for(std::size_t d = 1; d < info.dimension; ++d)
    os << delim << attr.get_scalar(d);
  return os;
}

void csv_printer::write_entry(const attribute_list &entry)
{
  std::vector<column_info>::const_iterator it = m_infos.begin(),
                                           end = m_infos.end();

  std::stringstream buf;

  if(end == it)
    return (void)(buf << std::endl);

  s_write_data_part(buf, entry[it->index], *it, m_delimiter);

  for(it = std::next(it); end != it; ++it)
  {
    buf << m_delimiter;
    s_write_data_part(buf, entry[it->index], *it, m_delimiter);
  }
  buf << std::endl;

  ostream() << buf.str();
}

void csv_printer::write_footer() { ostream() << std::endl; }
END_NAMESPACE_CORE
