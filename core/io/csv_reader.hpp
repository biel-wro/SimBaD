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
  csv_reader(std::istream *istream, property_tree const &pt);
  csv_reader(std::istream *istream, std::string const &delimiter = ",",
             std::string const &numsep = "_");
  ~csv_reader();

  attribute_description read_header() override;

  void visit_entries(entry_visitor v, std::size_t max_reads=0) override;

private:
  attribute_description m_attribute_description;
  std::vector<std::size_t> m_attribute_sizes;

  struct header_parser;
  struct record_parser;

  std::unique_ptr<header_parser> m_header_parser_ptr;
  std::unique_ptr<record_parser> m_record_parser_ptr;
};
END_NAMESPACE_CORE

#endif // SIMBAD_CSV_READER_HPP
