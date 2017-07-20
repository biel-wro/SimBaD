#include <interface/attribute_description.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <string>
template <class Iterator>
static bool parse_csv_header(Iterator first, Iterator last,
                             std::vector<std::string> &headers)
{
  namespace qi = boost::spirit::qi;
/*
  qi::rule<Iterator, std::string> quoted_string =
      '"' >> *(qi::char_ - '"') >> '"';
  qi::rule<Iterator, std::string> valid_characters = qi::char_ - '"' - ',';
  qi::rule<std::string::const_iterator, std::string()> header =
      *(quoted_string | valid_characters);

  qi::rule<std::string::const_iterator, std::vector<std::string>()> csv_parser =
      header % ',';

  return qi::parse(first, last, csv_parser, headers);
*/
}

BOOST_AUTO_TEST_CASE(experimenting)
{
  std::string testval = "Position,time,\"life.span\"";
  std::cout << "test string:" << std::endl;
  std::cout << testval << std::endl;

  std::string::const_iterator first = testval.begin();
  std::string::const_iterator last = testval.end();

  std::vector<std::string> result;
  //bool ok = parse_csv_header(first, last, result);

  std::cout << "result:" << std::endl;
}