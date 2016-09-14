#include "lazy_set.hpp"
#include "lazy_set_chunk.hpp"
#include "lazy_set_impl.hpp"

#include <boost/test/auto_unit_test.hpp>

#include <string>

typedef simbad::core::LazySet<unsigned> int_set;
typedef simbad::core::LazySet<std::string> string_set;

BOOST_AUTO_TEST_CASE(construct_and_destruct)
{
  string_set ss;
  int_set is;

  string_set::iterator sit;
  string_set::const_iterator scit;

  typename int_set::iterator iit;
  typename int_set::const_iterator icit;

  string_set const &css = ss;
  (void)css.begin();
  (void)css.end();

  int_set const &cis = is;
  (void)cis.begin();
  (void)css.end();
}

BOOST_AUTO_TEST_CASE(fill_and_erase)
{
  const unsigned TEST_SIZE = 5 * 1024;
  string_set ss;
  int_set is;

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    BOOST_TEST_CHECKPOINT("i=" << i);
    is.emplace_back(i);
    ss.emplace_back(std::to_string(i));
  }

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    BOOST_TEST_CHECKPOINT("i=" << i);

    BOOST_REQUIRE_EQUAL(is.size(), TEST_SIZE - i);
    is.pop_back();

    BOOST_REQUIRE_EQUAL(ss.size(), TEST_SIZE - i);
    ss.pop_back();
  }
  BOOST_REQUIRE_EQUAL(is.size(), 0);
  BOOST_REQUIRE_EQUAL(ss.size(), 0);
}

BOOST_AUTO_TEST_CASE(lazy_set_visitor)
{
  const unsigned TEST_SIZE = 5678;
  string_set ss;

  for (unsigned i = 0; i < TEST_SIZE; ++i)
    ss.emplace_back(std::to_string(i));

  std::vector<bool> visited_ss(TEST_SIZE, false);
  auto string_visitor = [&visited_ss](std::string &str) {
    unsigned idx = std::stoi(str);
    BOOST_REQUIRE(!visited_ss.at(idx));
    visited_ss.at(idx) = true;
    str = "visited";
  };
  ss.visit(string_visitor);

  for (bool was_visited : visited_ss)
    BOOST_REQUIRE(was_visited);
}

BOOST_AUTO_TEST_CASE(lazy_set_const_visitor)
{
  const unsigned TEST_SIZE = 8765;
  int_set is;

  for (unsigned i = 0; i < TEST_SIZE; ++i)
    is.emplace_back(i);

  std::vector<bool> visited_int(TEST_SIZE, false);
  auto int_visitor = [&visited_int](int const &v) {
    unsigned idx = int(v);
    BOOST_REQUIRE(!visited_int.at(v));
    visited_int.at(idx) = true;
  };

  static_cast<int_set const &>(is).visit(int_visitor);
  for (bool was_visited : visited_int)
    BOOST_REQUIRE(was_visited);
}

BOOST_AUTO_TEST_CASE(iterator_sweep)
{
  const unsigned TEST_SIZE = 5 * 1024;
  string_set ss;
  int_set is;
  BOOST_TEST_CHECKPOINT("filling");
  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    is.emplace_back(i);
    ss.emplace_back(std::to_string(i));
  }

  BOOST_TEST_CHECKPOINT("forward pass");
  auto iis = is.begin();
  auto sis = ss.begin();
  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    BOOST_TEST_CHECKPOINT("forward i=" << i);
    BOOST_REQUIRE_EQUAL(*iis, i);
    ++iis;

    BOOST_REQUIRE_EQUAL(*sis, std::to_string(i));
    ++sis;
  }
  BOOST_REQUIRE(iis == is.end());
  BOOST_REQUIRE(sis == ss.end());

  BOOST_TEST_CHECKPOINT("backward pass");
  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    BOOST_TEST_CHECKPOINT("backward i=" << i);
    --iis;
    BOOST_REQUIRE_EQUAL(*iis, TEST_SIZE - i - 1);

    --sis;
    BOOST_REQUIRE_EQUAL(*sis, std::to_string(TEST_SIZE - i - 1));
  }
}

BOOST_AUTO_TEST_CASE(find)
{
  const unsigned TEST_SIZE = 5 * 1024;
  string_set ss;
  int_set is;

  BOOST_TEST_CHECKPOINT("filling");
  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    is.emplace_back(i);
    ss.emplace_back(std::to_string(i));
  }

  string_set const &cis = ss;
  auto iit = is.begin();
  auto sit = cis.begin();

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    auto ifit = is.find(*iit);
    BOOST_REQUIRE(ifit == iit);
    ++iit;

    auto sfit = ss.find(*sit);
    BOOST_REQUIRE(sfit == sit);
    ++sit;
  }
}
