//#define BOOST_SPIRIT_DEBUG
#ifndef SIMBAD_CSV_HEADER_GRAMMAR_HPP
#define SIMBAD_CSV_HEADER_GRAMMAR_HPP

#include "core_def.hpp"

#include <boost/fusion/include/std_pair.hpp>
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
    m_singleton_column = qi::lexeme['"' >> +(qi::char_ - '"') >> '"'] |
                         qi::lexeme['\'' >> +(qi::char_ - '\'') >> '\''] |
                         qi::lexeme[+(qi::char_ - delim)];

    m_column_0_unquoted =
        qi::lexeme[*(qi::char_ - delim - numsep) >> numsep >> '0'];
    m_column_0_quoted =
        qi::lexeme['"' >> *(qi::char_ - numsep) >> numsep >> '0' >> '"'];
    m_column_0 = qi::hold[m_column_0_quoted] | m_column_0_unquoted;

    using col = qi::uint_parser<std::size_t>;

    m_column_n_unquoted =
        qi::lexeme[qi::lit(qi::_r1) >> numsep >> col()(qi::_r2)];
    m_column_n_quoted =
        qi::lexeme['"' >> qi::lit(qi::_r1) >> numsep >> col()(qi::_r2) >> '"'];

    m_column_n = qi::hold[m_column_n_quoted(qi::_r1, qi::_r2)] |
                 m_column_n_unquoted(qi::_r1, qi::_r2);

    m_remaining_partial_columns %=
        qi::eps[qi::_val = 1] >>
        *(delim >> m_column_n(qi::_r1, qi::_val)[qi::_val += 1]);

    m_merged_columns %=
        m_column_0[qi::_a = qi::_1] >> m_remaining_partial_columns(qi::_a);
    m_attribute =
        qi::hold[m_merged_columns] | (m_singleton_column >> qi::attr(1));

    m_start = m_attribute % delim;

    BOOST_SPIRIT_DEBUG_NODE(m_start);
    BOOST_SPIRIT_DEBUG_NODE(m_attribute);
    BOOST_SPIRIT_DEBUG_NODE(m_merged_columns);
    BOOST_SPIRIT_DEBUG_NODE(m_remaining_partial_columns);

    BOOST_SPIRIT_DEBUG_NODE(m_column_0);
    BOOST_SPIRIT_DEBUG_NODE(m_column_0_quoted);
    BOOST_SPIRIT_DEBUG_NODE(m_column_0_unquoted);

    BOOST_SPIRIT_DEBUG_NODE(m_column_n);
    BOOST_SPIRIT_DEBUG_NODE(m_column_n_quoted);
    BOOST_SPIRIT_DEBUG_NODE(m_column_n_unquoted);

    BOOST_SPIRIT_DEBUG_NODE(m_singleton_column);
  }
  template <class... T> using rule = boost::spirit::qi::rule<T...>;
  template <class... T> using locals = boost::spirit::qi::locals<T...>;
  using space_type = boost::spirit::ascii::space_type;

  using pair_type = std::pair<std::string, std::size_t>;
  using result_type = std::vector<pair_type>;

  rule<Iterator, result_type(), space_type> m_start;
  rule<Iterator, pair_type(), space_type> m_attribute;
  rule<Iterator, locals<std::string>, pair_type(), space_type> m_merged_columns;

  rule<Iterator, std::string(), space_type> m_column_0;
  rule<Iterator, std::string(), space_type> m_column_0_unquoted;
  rule<Iterator, std::string(), space_type> m_column_0_quoted;

  rule<Iterator, void(std::string, std::size_t), space_type> m_column_n;
  rule<Iterator, void(std::string, std::size_t), space_type> m_column_n_quoted;
  rule<Iterator, void(std::string, std::size_t), space_type>
      m_column_n_unquoted;

  rule<Iterator, std::size_t(std::string), space_type>
      m_remaining_partial_columns;
  rule<Iterator, std::string(), space_type> m_singleton_column;
};
END_NAMESPACE_CORE

#endif
