#include "lazy_set_chunk.hpp"

#include <boost/test/auto_unit_test.hpp>

#include <string>

typedef simbad::core::LazySetChunk<std::string> string_chunk;
typedef simbad::core::LazySetChunk<unsigned> int_chunk;

BOOST_AUTO_TEST_CASE(LazySetChunk_construct)
{
  string_chunk schk;
  (void)schk;
  int_chunk ichk;
  (void)ichk;
}

BOOST_AUTO_TEST_CASE(LazySetChunk_fill_and_pop)
{
  string_chunk schk;
  int_chunk ichk;

  BOOST_CHECK(ichk.isEmpty());
  BOOST_CHECK(schk.isEmpty());

  for (unsigned i = 0; i < ichk.length(); ++i)
  {
    BOOST_CHECK_EQUAL(ichk.occupancy(), i);

    ichk.emplace_back(i);
    BOOST_CHECK_EQUAL(ichk.back(), i);

    schk.emplace_back(std::to_string(i));
    BOOST_CHECK_EQUAL(schk.back(), std::to_string(i));

    BOOST_CHECK_EQUAL(ichk.occupancy(), i + 1);
  }

  BOOST_CHECK(ichk.isFull());
  BOOST_CHECK(schk.isFull());

  for (unsigned i = 0; i < ichk.length(); ++i)
  {
    BOOST_REQUIRE_EQUAL(ichk.at(i), i);
    BOOST_REQUIRE_EQUAL(schk.at(i), std::to_string(i));
  }

  for (unsigned i = ichk.length() - 1; i > 0; --i)
  {
    BOOST_REQUIRE_EQUAL(ichk.back(), i);
    ichk.pop_back();

    BOOST_REQUIRE_EQUAL(schk.back(), std::to_string(i));
    schk.pop_back();
  }
  ichk.pop_back();
  schk.pop_back();

  BOOST_CHECK(ichk.isEmpty());
  BOOST_CHECK(schk.isEmpty());
}

BOOST_AUTO_TEST_CASE(LazySetChunk_iterators)
{
  string_chunk chk;

  for (unsigned i = 0; i < chk.length(); ++i)
  {
    chk.emplace_back(std::to_string(i));
  }

  auto it = chk.begin();

  for (unsigned i = 0; i < chk.length(); ++i, ++it)
  {
    BOOST_CHECK_NE(it-chk.begin(), chk.end() - chk.begin());
    BOOST_CHECK_EQUAL(*it, std::to_string(i));
  }
  BOOST_CHECK_EQUAL(it, chk.end());
}

BOOST_AUTO_TEST_CASE(LazySetChunk_const_visitor)
{
  string_chunk chk;
  unsigned test_len = chk.length() - 10;

  for (unsigned i = 0; i < test_len; ++i)
    chk.emplace_back(std::to_string(i));

  string_chunk const &cchk(chk);

  std::vector<bool> visited(test_len, false );

  auto const_visitor = [&visited](std::string const &str){
    unsigned idx = std::stoi( str );
    BOOST_REQUIRE(!visited[idx]);
    visited[idx] = true;
  };
  cchk.visit(const_visitor);
  for(bool was_visited : visited)
      BOOST_REQUIRE(was_visited);
}

BOOST_AUTO_TEST_CASE(LazySetChunk_visitor)
{
  string_chunk chk;
  unsigned test_len = chk.length() - 10;

  for (unsigned i = 0; i < test_len; ++i)
    chk.emplace_back(std::to_string(i));

  std::vector<bool> visited(test_len, false );

  auto visitor = [&visited](std::string &str){
    unsigned idx = std::stoi( str );
    str = "visited";
    BOOST_REQUIRE(!visited[idx]);
    visited[idx] = true;
  };
  chk.visit(visitor);
  for(bool was_visited : visited)
      BOOST_REQUIRE(was_visited);
}


BOOST_AUTO_TEST_CASE(LazySetChunk_find)
{
  string_chunk chk;

  for (unsigned i = 0; i < chk.length(); ++i)
  {
    chk.emplace_back(std::to_string(i));
  }

  auto it = chk.begin();
  for (unsigned i = 0; i < chk.length(); ++i, ++it)
  {
    BOOST_TEST_CHECKPOINT("i=" << i);

    std::string *ptr = &*it;
    BOOST_CHECK(chk.containsByAddress(ptr));

    unsigned idx = chk.findIdx(*ptr);
    BOOST_CHECK_LT(idx, chk.occupancy());
    BOOST_CHECK_EQUAL(idx, i);

    BOOST_CHECK_EQUAL(idx, it - chk.begin());
  }
}
