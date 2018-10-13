#ifndef CSV_PRINTER_HPP
#define CSV_PRINTER_HPP

#include "interface/stream_printer.hpp"

#include <string>
#include <unordered_map>
#include <vector>
BEGIN_NAMESPACE_CORE

class csv_printer final : public stream_printer
{
public:
  using precision_map = std::unordered_map<std::string, std::size_t>;
  static constexpr char default_delimiter[] = ",";
  static const std::size_t double_digits;

  explicit csv_printer(property_tree const &pt);
  csv_printer(property_tree const &pt, std::ostream *);
  explicit csv_printer(std::ostream *, std::string delimiter = ",",
                       std::size_t default_digits = double_digits,
                       precision_map special_precision_map = {});

  void write_header(attribute_description const &desc) override;
  void write_entry(attribute_list const &entry) override;
  void write_footer() override;
protected:
  struct column_info
  {
    std::size_t index;
    std::size_t dimension;
    std::size_t precision_digits;
  };
  std::ostream &s_write_data_part(std::ostream &, attribute const &,
                                  column_info const &,
                                  std::string const &delimiter);
private:
  std::string const m_delimiter;
  std::size_t const m_default_precison_digits;

  std::unordered_map<std::string, std::size_t> m_special_precison;


  std::vector<column_info> m_infos;
};

END_NAMESPACE_CORE
#endif // CSV_PRINTER_HPP
