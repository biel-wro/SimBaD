#ifndef SIMBAD_CORE_CSV_READER_HPP
#define SIMBAD_CORE_CSV_READER_HPP

#include "io/stream_reader.hpp"

#include <string>
#include <vector>


BEGIN_NAMESPACE_CORE
class csv_reader : public stream_reader
{
public:
  csv_reader(std::istream *istream, property_tree const &pt);
  csv_reader(std::istream *istream, std::string delimiter = ",");

  attribute_description read_header() override;
  attribute_list read_entry(attribute_description const &description) override;
  void read_footer(attribute_description const &description) override;

private:

};
END_NAMESPACE_CORE

#endif // SIMBAD_CSV_READER_HPP
