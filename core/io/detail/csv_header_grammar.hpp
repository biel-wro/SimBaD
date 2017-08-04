//#define BOOST_SPIRIT_DEBUG
#ifndef SIMBAD_CSV_HEADER_GRAMMAR_HPP
#define SIMBAD_CSV_HEADER_GRAMMAR_HPP

#include "core_def.hpp"

#include <boost/compressed_pair.hpp>

#include <boost/fusion/include/std_pair.hpp>
//#include <boost/fusion/include/tuple.hpp>

#include <boost/fusion/include/at_c.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>

BEGIN_NAMESPACE_CORE

template <class Iterator>
struct csv_header_grammar
    : public boost::spirit::qi::grammar<
          Iterator, std::vector<std::pair<std::string, std::size_t>>(),
          boost::spirit::ascii::space_type>
{
  csv_header_grammar(std::string const &delim = ",",
                     std::string const &numsep = "_")
      : csv_header_grammar::base_type(m_start)
  {
    namespace qi = boost::spirit::qi;
    m_singleton_column = qi::lexeme[+(qi::char_ - delim)] |
                         qi::lexeme['"' >> +(qi::char_ - '"') >> '"'] |
                         qi::lexeme['\'' >> +(qi::char_ - '\'') >> '\''];

    m_first_partial_column =
        qi::lexeme[*(qi::char_ - delim - numsep) >> numsep >> '0'];

    m_another_partial_column =
        qi::lexeme[qi::lit(qi::_r1) >> numsep >>
                   qi::uint_parser<std::size_t>()(qi::_r2)];

    m_remaining_partial_columns %=
        qi::eps[qi::_val = 1] >>
        *(delim >> m_another_partial_column(qi::_r1, qi::_val)[qi::_val += 1]);

    m_merged_columns %= m_first_partial_column[qi::_a = qi::_1] >>
                        m_remaining_partial_columns(qi::_a);
    m_attribute = qi::hold[m_merged_columns] | (m_singleton_column >> qi::attr(1));

    m_start = m_attribute % delim;

  /*  BOOST_SPIRIT_DEBUG_NODE(m_start);
    BOOST_SPIRIT_DEBUG_NODE(m_attribute);
    BOOST_SPIRIT_DEBUG_NODE(m_merged_columns);
    BOOST_SPIRIT_DEBUG_NODE(m_remaining_partial_columns);
    BOOST_SPIRIT_DEBUG_NODE(m_another_partial_column);
    BOOST_SPIRIT_DEBUG_NODE(m_first_partial_column);
    BOOST_SPIRIT_DEBUG_NODE(m_singleton_column);
  */}
  template <class... T> using rule = boost::spirit::qi::rule<T...>;
  template <class... T> using locals = boost::spirit::qi::locals<T...>;
  using space_type = boost::spirit::ascii::space_type;

  using pair_type = std::pair<std::string, std::size_t>;
  using result_type = std::vector<pair_type>;

  rule<Iterator, result_type(), space_type> m_start;
  rule<Iterator, pair_type(), space_type> m_attribute;
  rule<Iterator, locals<std::string>, pair_type(), space_type> m_merged_columns;
  rule<Iterator, std::string(), space_type> m_first_partial_column;
  rule<Iterator, void(std::string, std::size_t), space_type>
      m_another_partial_column;
  rule<Iterator, std::size_t(std::string), space_type>
      m_remaining_partial_columns;
  rule<Iterator, std::string(), space_type> m_singleton_column;
};
END_NAMESPACE_CORE

#endif
