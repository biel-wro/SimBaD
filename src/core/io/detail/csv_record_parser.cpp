#include "csv_record_parser.hpp"

#include "csv_record_grammar.hpp"
#include "interface/attribute.hpp"

BEGIN_NAMESPACE_CORE
template <class Iterator>
csv_record_parser<Iterator>::csv_record_parser(std::string const &delimiter)
    : m_grammar_ptr(new csv_record_grammar<iterator_type>(delimiter))
{
}

template <class Iterator> csv_record_parser<Iterator>::~csv_record_parser() {}
template <class Iterator>
bool csv_record_parser<Iterator>::parse(
    Iterator &first, Iterator last,
    const std::vector<std::size_t> &attribute_sizes,
    std::vector<attribute> &record) const
{
  return boost::spirit::qi::phrase_parse(
      first, last, (*m_grammar_ptr)(boost::phoenix::cref(attribute_sizes)),
      boost::spirit::ascii::space, record);
}

template class csv_record_parser<std::string::const_iterator>;

END_NAMESPACE_CORE
