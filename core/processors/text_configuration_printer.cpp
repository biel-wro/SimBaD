#include "text_configuration_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
#include "interface/property_tree.hpp"
#include <cstddef>
#include <iostream>

BEGIN_NAMESPACE_CORE
text_configuration_printer::text_configuration_printer()
    : m_ostream_ptr(nullptr)
{
}
text_configuration_printer::text_configuration_printer(std::ostream &ostream)
    : m_ostream_ptr(&ostream)
{
}

text_configuration_printer::text_configuration_printer(const property_tree &pt)
    : text_configuration_printer()
{
  std::string stream_name = pt.get("output", "stdout");
  if("stdout" == stream_name)
    set_ostream(std::cout);
  else
    throw std::runtime_error("unrecognized output: " + stream_name);
}

void text_configuration_printer::set_ostream(std::ostream &ostream)
{
  m_ostream_ptr = &ostream;
}

static void print_id(std::ostream &os, particle const &p, bool has_id)
{
  if(has_id)
    os << p.id() << ": ";
}

static void print_coords(std::ostream &os, particle const &p, std::size_t dim)
{
  for(size_t d = 0; d < dim; ++d)
    os << p.coord(d) << " ";
}

static void print_atributes(std::ostream &os, particle const &p,
                            std::size_t nattrs,
                            std::vector<std::string> const &names,
                            std::vector<std::size_t> const &indices)
{
  os << "{";
  if(nattrs > 0)
    os << names[0] << "=" << p.get_attribute(indices[0]);
  for(std::size_t i = 1; i < nattrs; ++i)
    os << ", " << names[i] << "=" << p.get_attribute(indices[i]);
  os << "}" << std::endl;
}

void text_configuration_printer::read_configuration(
    const configuration_view &conf)
{
  size_t dimension = conf.dimension();
  bool has_id = conf.has_unique_id();

  std::size_t n_attributes = conf.attr_map().size();

  std::vector<std::size_t> attr_indices;
  std::vector<std::string> attr_names;
  std::tie(attr_indices,attr_names) = conf.attr_map().unpack_all();

  conf.visit_configuration([&](particle const &p) {
    print_id(*m_ostream_ptr, p, has_id);
    print_coords(*m_ostream_ptr, p, dimension);
    print_atributes(*m_ostream_ptr, p, n_attributes, attr_names, attr_indices);
  });
}

END_NAMESPACE_CORE
