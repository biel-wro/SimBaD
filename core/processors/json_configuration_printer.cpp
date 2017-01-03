#include "json_configuration_printer.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"
#include <cstddef>
#include <iostream>


BEGIN_NAMESPACE_CORE

static void print_id(std::ostream &os, particle const &p, bool has_id)
{
  if(has_id)
    os << "\"id\":" << p.id() << ",";
}
static void print_coords(std::ostream &os, particle const &p,
                             std::size_t dim)
{
  os << "[";
  if(dim > 0)
    os << p.coord(0);
  for(size_t d = 1; d < dim; ++d)
    os << "," << p.coord(d);
  os << "]";
}

static void print_atributes(std::ostream &os, particle const &p,
                            std::size_t nattrs,
                            std::vector<std::string> const &names,
                            std::vector<std::size_t> const &indices)
{
  os << ",[";
  if(nattrs > 0)
    os << names[0] << ":" << p.get_attribute(indices[0]);
  for(std::size_t i = 1; i < nattrs; ++i)
    os << "," << names[i] << ":" << p.get_attribute(indices[i]);
  os << "]" << std::endl;
}

json_configuration_printer::json_configuration_printer(std::ostream &ostream)
  : stream_printer(ostream)
{

}

void json_configuration_printer::read_configuration(
    const configuration_view &conf)
{
  size_t dimension = conf.dimension();
  bool has_id = conf.has_unique_id();

  std::size_t n_attributes = conf.attr_map().size();

  std::vector<std::size_t> attr_indices;
  std::vector<std::string> attr_names;
  std::tie(attr_indices,attr_names) = conf.attr_map().unpack_all();

  conf.visit_configuration([&](particle const &p) {
    print_id(ostream(), p, has_id);
    print_coords(ostream(), p, dimension);
    print_atributes(ostream(), p, n_attributes, attr_names, attr_indices);
  });
}

END_NAMESPACE_CORE
