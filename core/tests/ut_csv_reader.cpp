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
/*
BOOST_AUTO_TEST_CASE(number_list)
{
  std::string test_val = "1.0, 3.4, 2.3";

  std::vector<double> result;
  bool r = qi::phrase_parse(test_val.begin(), test_val.end(),
                            //  Begin grammar
                            (qi::double_ % ','),
                            //  End grammar
                            qi::space, result);
  BOOST_REQUIRE(r);
}

struct quoted_string : public qi::grammar<It, std::string()>
{
  quoted_string() : quoted_string::base_type(start)
  {
    start %= qi::lexeme['"' >> +(qi::char_ - '"') >> '"'];
  }
  qi::rule<It, std::string()> start;
};

BOOST_AUTO_TEST_CASE(quoted_string_test)
{
  std::string test_val = "\"A quoted string\"";
  It first = test_val.begin(), last = test_val.end();
  std::string result;

  quoted_string parser;
  bool r = qi::phrase_parse(first, last, parser, qi::space, result);
  BOOST_REQUIRE(first == last);
  BOOST_REQUIRE(r);
}
struct string_with_underline
    : public qi::grammar<It, std::tuple<std::string, std::string>()>
{
  string_with_underline() : string_with_underline::base_type(start)
  {
    name = *(qi::char_  - '_');
    name2 = *(qi::char_ );
    start = name >> '_'>> name2;
  }
  qi::rule<It, std::string()> name;
  qi::rule<It, std::string()> name2;
  qi::rule<It, std::tuple<std::string, std::string>()> start;
};

BOOST_AUTO_TEST_CASE(string_with_underline_test)
{
  std::string test_val = "blabla_bla";
  It first = test_val.begin(), last = test_val.end();
  std::tuple<std::string, std::string> result;

  string_with_underline parser;
  bool r = qi::parse(first, last, parser, result);
  std::cout << "result:" << std::endl;
  std::cout << std::get<0>(result) << std::endl;
  std::cout << std::get<1>(result) << std::endl;

  BOOST_REQUIRE(first == last);
  BOOST_REQUIRE(r);
}
//typedef std::map<std::string, std::string> pairs_type;
using result_type = std::tuple<std::string, std::string>;
template <typename Iterator>
struct key_value_sequence : qi::grammar<Iterator, result_type()>
{
  key_value_sequence() : key_value_sequence::base_type(pair)
  {
    //query = pair >> *((qi::lit(';') | '&') >> pair);

    key = qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
    value = +qi::char_("a-zA-Z_0-9");
    pair = key >> "=" >> value;
  }

//  qi::rule<Iterator, pairs_type()> query;
  qi::rule<Iterator, result_type()> pair;
  qi::rule<Iterator, std::string()> key, value;
 // qi::rule<Iterator, char()> delimiter;
};

BOOST_AUTO_TEST_CASE(key_value_test)
{
  std::string input("key1=value1");
  std::string::const_iterator begin = input.begin();
  std::string::const_iterator end = input.end();

  //key_value_sequence<std::string::iterator> p;
  key_value_sequence<std::string::const_iterator> parser;
  result_type m;

  if(!qi::parse(begin, end, parser, m))
  {
    std::cout << "-------------------------------- \n";
    std::cout << "Parsing failed\n";
    std::cout << "-------------------------------- \n";
  }
  else
  {
    std::cout << "-------------------------------- \n";
    std::cout << "Parsing succeeded, found entries:\n";
   // pairs_type::iterator end = m.end();
    //for(pairs_type::iterator it = m.begin(); it != end; ++it)
    {
      std::cout << std::get<0>(m);
      std::cout << "=" << std::get<1>(m);
      std::cout << std::endl;
    }
    std::cout << "---------------------------------\n";
  }
}*/
// using pair_type = std::pair<std::string, std::size_t>;
struct pair_type
{
  explicit pair_type(std::string const &name = "", std::size_t size = 0)
      : name(name), size(size)
  {
  }
  std::string name;
  std::size_t size;
};
BOOST_FUSION_ADAPT_STRUCT(pair_type,          //
                          (std::string, name) //
                          (std::size_t, size) //
)

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
    std::cout << pair.name << " " << pair.size << std::endl;

  BOOST_REQUIRE(ok);
  BOOST_REQUIRE(begin == end);
}