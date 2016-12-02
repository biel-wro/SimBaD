#include "text_configuration_printer.hpp"

#include "property_tree.hpp"

#include "interface/configuration_view.hpp"
#include "interface/particle.hpp"
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
    const configuration_view &conf, property_tree const &)
{
  conf.visit_configuration([this](particle const &p) {
    *m_ostream_ptr << p.coord(0) << " " << p.coord(1) << std::endl;
  });
}

END_NAMESPACE_CORE
