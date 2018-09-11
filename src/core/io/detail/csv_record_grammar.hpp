//#define BOOST_SPIRIT_DEBUG
#ifndef SIMBAD_CORE_CSV_RECORD_GRAMMAR
#define SIMBAD_CORE_CSV_RECORD_GRAMMAR
#include "core_def.hpp"

#include "interface/attribute.hpp"

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
template <class Iterator>
struct csv_record_grammar
    : public boost::spirit::qi::grammar<
          Iterator, std::vector<core::attribute>(std::vector<std::size_t>),
          boost::spirit::ascii::space_type>
{
  using attribute = core::attribute; // there also is one in qi::grammar
  using attr_list = std::vector<attribute>;
  using desc_type = std::vector<std::size_t>;

  using int_type = attribute::int_type;
  using real_type = attribute::real_type;

  csv_record_grammar(std::string const &delim = ",")
      : csv_record_grammar::base_type(m_start)
  {
    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;

    m_string = ('"' >> *(qi::char_ - '"') >> '"')   //
               | ("'" >> *(qi::char_ - "'") >> "'") //
               | (*(qi::char_ - delim));

    m_int = qi::int_parser<int_type>() >>
            !(qi::lit(".") | qi::lit("e") | qi::lit("E"));

    m_string_list = m_string >> qi::repeat(qi::_r1 - 1)[delim >> m_string];

    m_real_list = qi::double_ >> qi::repeat(qi::_r1 - 1)[delim >> qi::double_];

    m_int_list = m_int >> qi::repeat(qi::_r1 - 1)[delim >> m_int];

    m_int_attr = m_int_list(qi::_r1);

    m_attribute =
        m_int_attr(qi::_r1) | m_real_list(qi::_r1) | m_string_list(qi::_r1);

    m_attributes =
        m_attribute(qi::_r1[0])[phx::push_back(qi::_val, qi::_1)] //
        >> qi::eps[qi::_a = 1]                                    //
        >>
        qi::repeat(phx::size(qi::_r1) - 1) //
            [delim >>
             m_attribute(qi::_r1[qi::_a])[phx::push_back(qi::_val, qi::_1)] //
             >> qi::eps[qi::_a++]]                                          //
        >> -qi::lit(delim);

    m_start = m_attributes(qi::_r1);

    BOOST_SPIRIT_DEBUG_NODE(m_start);
    BOOST_SPIRIT_DEBUG_NODE(m_attribute);
    BOOST_SPIRIT_DEBUG_NODE(m_int_list);
    BOOST_SPIRIT_DEBUG_NODE(m_real_list);
    BOOST_SPIRIT_DEBUG_NODE(m_string_list);
    BOOST_SPIRIT_DEBUG_NODE(m_string);
    BOOST_SPIRIT_DEBUG_NODE(m_int);
  }

  template <class... T> using rule = boost::spirit::qi::rule<T...>;
  template <class... T> using locals = boost::spirit::qi::locals<T...>;
  using space_type = boost::spirit::ascii::space_type;

  using real_list = attribute::realn_type;
  using int_list = attribute::intn_type;
  using string_list = attribute::stringn_type;

  rule<Iterator, attr_list(desc_type), space_type> m_start;
  rule<Iterator, locals<std::size_t>, attr_list(desc_type), space_type>
      m_attributes;
  rule<Iterator, attribute(std::size_t), space_type> m_attribute;
  rule<Iterator, real_list(std::size_t), space_type> m_real_list;
  rule<Iterator, int_list(std::size_t), space_type> m_int_list;
  rule<Iterator, attribute(std::size_t), space_type> m_int_attr;
  rule<Iterator, string_list(std::size_t), space_type> m_string_list;

  rule<Iterator, attribute::int_type()> m_int;
  rule<Iterator, std::string()> m_string;
};

END_NAMESPACE_CORE

#endif // CSV_RECORD_GRAMMAR_HPP
