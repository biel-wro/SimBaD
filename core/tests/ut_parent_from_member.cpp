#include "parent_from_member.hpp"
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
namespace
{
// some complicated class
struct Base1
{
  std::vector<int> vec;
  char member_data[3];
  virtual ~Base1() {}
protected:
  char member_data2[7];
};

struct Base2 : public virtual Base1
{
  Base1 m1;
  char base_data1[1];
  Base1 m2;
  char base_data2[5];
  virtual ~Base2() {}
protected:
  char base_data3[1];
};

struct Base3 : public virtual Base1
{
  Base1 m1;
  char base_data1[1];
  Base1 m2;
  char base_data2[5];
  virtual ~Base3() {}
protected:
  char base_data3[1];
};

struct Parent : public Base2, public Base3
{
  Base1 member1;
  char pdata1[1];
  bool bits;
  Base2 member2;
  virtual ~Parent() {}
protected:
  char pdata2[2];
};
}

using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(ut_parent_from_member)

BOOST_AUTO_TEST_CASE(parent_automatic)
{
  Parent p;
  BOOST_REQUIRE_EQUAL(parent_from_member(&p.member1, &Parent::member1), &p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&p.member2, &Parent::member2), &p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&p.bits, &Parent::bits), &p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&p.pdata1, &Parent::pdata1), &p);
}

BOOST_AUTO_TEST_CASE(parent_on_heap)
{
  Parent *p = new Parent();
  BOOST_REQUIRE_EQUAL(parent_from_member(&(*p).member1, &Parent::member1), p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&(*p).member2, &Parent::member2), p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&(*p).bits, &Parent::bits), p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&(*p).pdata1, &Parent::pdata1), p);
  delete p;
}
BOOST_AUTO_TEST_CASE(parent_on_heap_by_arrow)
{
  Parent *p = new Parent();
  BOOST_REQUIRE_EQUAL(parent_from_member(&p->member1, &Parent::member1), p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&p->member2, &Parent::member2), p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&p->bits, &Parent::bits), p);
  BOOST_REQUIRE_EQUAL(parent_from_member(&p->pdata1, &Parent::pdata1), p);
  delete p;
}

BOOST_AUTO_TEST_SUITE_END()
