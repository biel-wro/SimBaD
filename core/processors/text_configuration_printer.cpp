#include "text_configuration_printer.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/attribute_descriptor.hpp"
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
void text_configuration_printer::read_configuration(
    const configuration_view &conf)
{
  size_t dimension = conf.dimension();
  bool has_id = conf.has_unique_id();

  conf.visit_configuration([this, dimension, has_id, &conf](particle const &p) {

    if(has_id)
      *m_ostream_ptr << p.id() << ": ";

    for(size_t d = 0; d < dimension; ++d)
    {
      double coord = p.coord(d);
      *m_ostream_ptr << coord << " ";
    }
    *m_ostream_ptr << "{";
    for(attribute_descriptor const &attrdesc : conf.attr_map())
    {
      *m_ostream_ptr << attrdesc.attribute_name() << "=";
      *m_ostream_ptr << p.get_attribute(attrdesc.attribute_idx()) << ", ";
    }
    *m_ostream_ptr << "}" << std::endl;
  });
}

END_NAMESPACE_CORE
