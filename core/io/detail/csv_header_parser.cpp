#include "csv_header_parser.hpp"

#include "csv_header_grammar.hpp"

BEGIN_NAMESPACE_CORE

template <class Iterator>
csv_header_parser<Iterator>::csv_header_parser(std::string const &delim,
                                               std::string const &numsep)
    : m_grammar_ptr(new csv_header_grammar<Iterator>(delim, numsep))
{
}

template <class Iterator>
bool csv_header_parser<Iterator>::parse(
    Iterator &first, Iterator last,
    std::vector<std::pair<std::string, std::size_t>> &result) const
{
  return boost::spirit::qi::phrase_parse(first, last, *m_grammar_ptr,
                                         boost::spirit::ascii::space, result);
}

template <class Iterator> csv_header_parser<Iterator>::~csv_header_parser() {}

template class csv_header_parser<std::string::const_iterator>;

END_NAMESPACE_CORE
