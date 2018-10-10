#ifndef SIMBAD_CORE_CSV_READER_HPP
#define SIMBAD_CORE_CSV_READER_HPP

#include "interface/stream_reader.hpp"

#include "interface/attribute_description.hpp"

#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
class csv_reader final : public stream_reader
{
public:
  constexpr static char DEFAULT_DELIMITER[] = ",";
  constexpr static char DEFAULT_SUBCOL_SEP[] = "_";

  explicit csv_reader(property_tree const &pt);
  explicit csv_reader(std::istream &stream_ref,
                      std::string const &delimiter = DEFAULT_DELIMITER,
                      std::string const &subcol_sep = DEFAULT_SUBCOL_SEP);

  explicit csv_reader(std::istream *istream_ptr,
                      std::string const &delimiter = DEFAULT_DELIMITER,
                      std::string const &subcolsep = DEFAULT_SUBCOL_SEP);
  explicit csv_reader(std::string const &filename,
                      std::string const &delimiter = DEFAULT_DELIMITER,
                      std::string const &numsep = DEFAULT_SUBCOL_SEP);
  ~csv_reader();

  attribute_description read_header() override;

  bool visit_entries(entry_visitor v, std::size_t max_reads = 0) override;

private:
  std::vector<std::size_t> m_attribute_sizes;

  struct header_parser;
  struct record_parser;

  std::unique_ptr<header_parser> m_header_parser_ptr;
  std::unique_ptr<record_parser> m_record_parser_ptr;
};
END_NAMESPACE_CORE

#endif // SIMBAD_CSV_READER_HPP
