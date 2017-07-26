#define BOOST_SPIRIT_USE_PHOENIX_V3
#define BOOST_SPIRIT_DEBUG
#include <interface/attribute_description.hpp>

#include <boost/compressed_pair.hpp>

#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <boost/fusion/include/at_c.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <string>
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;
namespace fusion = boost::fusion;
using pair_type = std::pair<std::string,std::size_t>;
/*
BOOST_FUSION_ADAPT_STRUCT(pair_type,          //
                          (std::string, name) //
                          (std::size_t, size) //
)*/

using result_type = std::vector<pair_type>;

using Iterator = std::string::const_iterator;
struct header_grammar
    : public qi::grammar<Iterator, result_type(), ascii::space_type>
{
  header_grammar(std::string const &delim = ",",
                 std::string const &numsep = "_")
      : header_grammar::base_type(start)
  {
    singleton_column = qi::lexeme[+(qi::char_ - delim)] |
                       qi::lexeme['"' >> +(qi::char_ - '"') >> '"'] |
                       qi::lexeme['\'' >> +(qi::char_ - '\'') >> '\''];

    first_partial_column =
        qi::lexeme[*(qi::char_ - delim - numsep) >> numsep >> '0'];

    another_partial_column =
        qi::lexeme[qi::lit(qi::_r1) >> numsep >>
                   qi::uint_parser<std::size_t>()(qi::_r2)];

    remaining_partial_columns %=
        qi::eps[qi::_val = 1] >>
        *(delim >> another_partial_column(qi::_r1, qi::_val)[qi::_val += 1]);

    merged_columns %= first_partial_column[qi::_a = qi::_1] >>
                      remaining_partial_columns(qi::_a);
    attribute = merged_columns | (singleton_column >> qi::attr(1));

    start = attribute % delim;

    BOOST_SPIRIT_DEBUG_NODE(start);
    BOOST_SPIRIT_DEBUG_NODE(attribute);
    BOOST_SPIRIT_DEBUG_NODE(merged_columns);
    BOOST_SPIRIT_DEBUG_NODE(remaining_partial_columns);
    BOOST_SPIRIT_DEBUG_NODE(another_partial_column);
    BOOST_SPIRIT_DEBUG_NODE(first_partial_column);
    BOOST_SPIRIT_DEBUG_NODE(singleton_column);
  }

  qi::rule<Iterator, result_type(), ascii::space_type> start;
  qi::rule<Iterator, pair_type(), ascii::space_type> attribute;
  qi::rule<Iterator, qi::locals<std::string>, pair_type(), ascii::space_type>
      merged_columns;
  qi::rule<Iterator, std::string(), ascii::space_type> first_partial_column;
  qi::rule<Iterator, void(std::string, std::size_t), ascii::space_type>
      another_partial_column;
  qi::rule<Iterator, std::size_t(std::string), ascii::space_type>
      remaining_partial_columns;
  qi::rule<Iterator, std::string(), ascii::space_type> singleton_column;
};

BOOST_AUTO_TEST_CASE(key_value_test)
{
  std::string input("position_0,position_1, position_2 , death.rate, life_0, life_1, bad_one_1");
  std::string::const_iterator begin = input.begin();
  std::string::const_iterator end = input.end();
  result_type r;
  header_grammar parser;
  bool ok = qi::phrase_parse(begin, end, parser, ascii::space, r);
  for(pair_type const &pair : r)
    std::cout << pair.first << " " << pair.second << std::endl;

  BOOST_REQUIRE(ok);
  BOOST_REQUIRE(begin == end);
}