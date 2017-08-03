#define BOOST_SPIRIT_USE_PHOENIX_V3
#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_DEBUG_TRACENODE 1

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "io/csv_reader.hpp"

#include <boost/optional.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

using attribute = simbad::core::attribute;
using attribute_description = simbad::core::attribute_description;
using csv_reader = simbad::core::csv_reader;

using opt_descriptor =
    boost::optional<simbad::core::attribute_descriptor const &>;

BOOST_AUTO_TEST_CASE(read_header)
{
  std::string test_value = "aaa_0; aaa_1; aaa_2; bb_0; bb_1; c";
  std::istringstream test_stream(test_value);

  simbad::core::csv_reader reader(&test_stream, ";");
  attribute_description description = reader.read_header();

  opt_descriptor desc_aaa = description.get_descriptor("aaa");
  BOOST_REQUIRE(desc_aaa);
  BOOST_REQUIRE_EQUAL(desc_aaa->attribute_name(), "aaa");
  BOOST_REQUIRE_EQUAL(desc_aaa->attribute_idx(), 0);
  BOOST_REQUIRE_EQUAL(desc_aaa->attribute_dimension(), 3);

  opt_descriptor desc_bb = description.get_descriptor("bb");
  BOOST_REQUIRE(desc_bb);
  BOOST_REQUIRE_EQUAL(desc_bb->attribute_name(), "bb");
  BOOST_REQUIRE_EQUAL(desc_bb->attribute_idx(), 1);
  BOOST_REQUIRE_EQUAL(desc_bb->attribute_dimension(), 2);

  opt_descriptor desc_c = description.get_descriptor("c");
  BOOST_REQUIRE(desc_c);
  BOOST_REQUIRE_EQUAL(desc_c->attribute_name(), "c");
  BOOST_REQUIRE_EQUAL(desc_c->attribute_idx(), 2);
  BOOST_REQUIRE_EQUAL(desc_c->attribute_dimension(), 1);
}

/*
#include <boost/compressed_pair.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/phoenix/stl/container/container.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
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

using attr = simbad::core::attribute;
using attr_list = std::vector<attr>;
using desc_type = std::vector<std::size_t>;

using Iterator = std::string::const_iterator;

using real_type = double;
using int_type = std::int64_t;

using string_list = simbad::core::array_attribute<std::string>;
using int_list = simbad::core::array_attribute<int_type>;
using real_list = simbad::core::array_attribute<real_type>;

struct record_grammar
    : public qi::grammar<Iterator, attr_list(desc_type), ascii::space_type>
{
  record_grammar(std::string const &delim = ",")
      : record_grammar::base_type(m_start)
  {
    m_string = ('"' >> *(qi::char_ - '"') >> '"')   //
               | ("'" >> *(qi::char_ - "'") >> "'") //
               | (*(qi::char_ - delim));

    m_string_list = m_string >> qi::repeat(qi::_r1 - 1)[delim >> m_string];

    m_real_list = qi::double_ >> qi::repeat(qi::_r1 - 1)[delim >> qi::double_];

    m_int_list = qi::long_ >> !qi::lit(".") >>
                 qi::repeat(qi::_r1 - 1)[delim >> qi::long_ >> !qi::lit(".")];
    m_int_attr = m_int_list(qi::_r1);

    m_attribute = m_int_attr(qi::_r1) |  //
                  m_real_list(qi::_r1) | //
                  m_string_list(qi::_r1);

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
  }

  qi::rule<Iterator, attr_list(desc_type), ascii::space_type> m_start;
  qi::rule<Iterator, qi::locals<std::size_t>, attr_list(desc_type),
           ascii::space_type>
      m_attributes;
  qi::rule<Iterator, attr(std::size_t), ascii::space_type> m_attribute;
  qi::rule<Iterator, real_list(std::size_t), ascii::space_type> m_real_list;
  qi::rule<Iterator, int_list(std::size_t), ascii::space_type> m_int_list;
  qi::rule<Iterator, attr(std::size_t), ascii::space_type> m_int_attr;
  qi::rule<Iterator, string_list(std::size_t), ascii::space_type> m_string_list;

  qi::rule<Iterator, std::string()> m_string;
};

BOOST_AUTO_TEST_CASE(key_value_test)
{
  record_grammar grammar;
  std::string test_string =
      "1, 2, 3, 5.0, 3.0, 'abc', \"def\", 'ghi', 1, 3.14,";
  Iterator first = test_string.begin(), last = test_string.end();

  // std::size_t n = 3;
  desc_type widths{3, 2, 3, 1, 1};
  attr_list outcome;
  bool ok = qi::phrase_parse(first, last, grammar(phx::cref(widths)),
                             ascii::space, outcome);

  std::cout << "outcome: " << std::endl;

  for(attr const &a : outcome)
    std::cout << "  " << a << std::endl;

  BOOST_REQUIRE(first == last);
  BOOST_REQUIRE(ok);
}*/
