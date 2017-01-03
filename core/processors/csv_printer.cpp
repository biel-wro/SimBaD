#include "csv_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
BEGIN_NAMESPACE_CORE

csv_printer::csv_printer(std::ostream *ostream, const property_tree &pt)
    : csv_printer(ostream, pt.get<std::string>("delimiter", ","))
{
}
csv_printer::csv_printer(std::ostream *ostream, std::string delimiter)
    : stream_printer(ostream), m_delimiter(std::move(delimiter))
{
}
static void print_header(std::ostream &os, configuration_view const &conf,
                         std::vector<std::string> const &names,
                         std::string const &delimiter)
{
  if(conf.has_unique_id())
    os << "id" << delimiter;

  std::size_t dimension = conf.dimension();
  for(std::size_t d = 0; d < dimension; ++d)
    os << "coord_" << d << delimiter;

  for(std::string const &name : names)
    os << "\"" << name << "\""<< delimiter;
  os << std::endl;
}

static void print_values(std::ostream &os, configuration_view const &conf,
                         std::vector<std::size_t> const &indices,
                         std::string const &delimiter)
{
  bool has_unique_id = conf.has_unique_id();
  std::size_t dimension = conf.dimension();

  conf.visit_configuration([&](particle const &p) {
    if(has_unique_id)
      os << p.id() << delimiter;

    for(std::size_t d = 0; d < dimension; ++d)
      os << p.coord(d) << delimiter;

    for(std::size_t idx : indices)
      os << p.get_attribute(idx) << delimiter;
    os << std::endl;
  });
}

void csv_printer::read_configuration(const configuration_view &conf)
{
  std::vector<std::size_t> indices;
  std::vector<std::string> names;
  std::tie(indices, names) = conf.attr_map().unpack_all();

  print_header(ostream(), conf, names, m_delimiter);
  print_values(ostream(), conf, indices, m_delimiter);
}
END_NAMESPACE_CORE


