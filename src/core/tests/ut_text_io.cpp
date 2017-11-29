#include <boost/config/warning_disable.hpp>
#include <boost/test/auto_unit_test.hpp>

//#include <boost/spirit/include/karma.hpp>

#include <iterator>
#include <vector>
BOOST_AUTO_TEST_SUITE(test_text_io)
/*
using namespace boost::spirit;

template <typename OutputIterator, typename Container>
bool generate_numbers(OutputIterator &sink, Container const &v)
{
  using karma::lit;
  using karma::double_;
  using karma::stream;
  using karma::generate_delimited;
  using ascii::space;

  bool r = karma::generate(sink, stream % lit(", "), v);
  return r;
}

BOOST_AUTO_TEST_CASE(printing)
{

  std::vector<double> vec{1, 2, 3, 4, 5};
  std::ostream_iterator<char> oi(std::cout);
  generate_numbers(oi, vec);
}*/
BOOST_AUTO_TEST_SUITE_END()
