#ifndef SIMBAD_NULL_PRINTER_HPP
#define SIMBAD_NULL_PRINTER_HPP

#include "interface/stream_printer.hpp"

BEGIN_NAMESPACE_CORE

class null_printer final : public stream_printer
{
public:
  explicit null_printer(property_tree const &);

  void write_header(attribute_description const &desc) override;
  void write_entry(attribute_list const &entry) override;
  void write_footer() override;
};

END_NAMESPACE_CORE


#endif //SIMBAD_NULL_PRINTER_HPP
