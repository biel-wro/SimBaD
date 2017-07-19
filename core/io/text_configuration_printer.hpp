#ifndef TEXT_CONFIGURATION_PRINTER_HPP
#define TEXT_CONFIGURATION_PRINTER_HPP

#include "stream_printer.hpp"

#include "core_fwd.hpp"

#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
class text_configuration_printer : public stream_printer
{
public:
  text_configuration_printer();
  text_configuration_printer(std::ostream *ostream,
                             std::string delimiter = ", ");
  text_configuration_printer(std::ostream *ostream, property_tree const &pt);

  void write_header(attribute_description const &desc) override;
  // void write_data(dataframe const &conf) override;
  void write_entry(attribute_list const &entry) override;
  void write_footer() override;

private:
  std::string m_delimiter;
  std::vector<std::string> m_names;
  std::vector<std::size_t> m_indices;
};

END_NAMESPACE_CORE
#endif // TEXT_CONFIGURATION_PRINTER_HPP
