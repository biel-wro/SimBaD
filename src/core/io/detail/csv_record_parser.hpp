#ifndef SIMBAD_CORE_CSV_RECORD_PARSER_HPP
#define SIMBAD_CORE_CSV_RECORD_PARSER_HPP
#include "core_fwd.hpp"

#include <memory>
#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
template <class> struct csv_record_grammar;

template <class Iterator> class csv_record_parser
{
public:
  using iterator_type = Iterator;

  csv_record_parser(std::string const &delimiter);
  ~csv_record_parser();
  bool parse(Iterator &first, Iterator last,
             std::vector<std::size_t> const &attribute_sizes,
             std::vector<attribute> &record) const;

private:
  std::unique_ptr<csv_record_grammar<iterator_type>> m_grammar_ptr;
};
END_NAMESPACE_CORE
#endif // CSV_RECORD_PARSER_HPP
