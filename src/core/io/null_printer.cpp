#include "null_printer.hpp"

BEGIN_NAMESPACE_CORE

null_printer::null_printer(const simbad::core::property_tree &) {}
void null_printer::write_header(attribute_description const &desc) {}
void null_printer::write_entry(attribute_list const &entry) {}
void null_printer::write_footer() {}
END_NAMESPACE_CORE
