#ifndef SIMBAD_CSV_HEADER_PARSER_HPP
#define SIMBAD_CSV_HEADER_PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "core_def.hpp"
BEGIN_NAMESPACE_CORE
template <class> struct csv_header_grammar;

template <class Iterator> struct csv_header_parser
{
  csv_header_parser(std::string const &delim, std::string const &numsep);
  bool parse(Iterator &first, Iterator last,
             std::vector<std::pair<std::string, std::size_t>> &result) const;
  ~csv_header_parser();

private:
  std::unique_ptr<csv_header_grammar<Iterator>> m_grammar_ptr;
};
END_NAMESPACE_CORE

#endif // SIMBAD_CSV_HEADER_PARSER_HPP
