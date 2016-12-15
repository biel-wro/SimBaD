#include <boost/test/auto_unit_test.hpp>

#include "interface/attribute.hpp"
#include "utils/attribute_converter.hpp"

#include "coordinates/coordinates.hpp"

#include <unordered_set>
#include <vector>

using namespace boost;
using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_attributes)

BOOST_AUTO_TEST_CASE(layout_preview)
{
  attribute attr0("text val");
  attribute attr1("quite long std::string exceedig 24 characters");
  attribute attr2(10ll);
  attribute attr3(20.0);
  attribute attr4(std::vector<long long int>{1, 2, 3});
  attribute attr5(std::vector<double>{10.0, 20.0, 30.0});
  attribute attr6(std::vector<long long int>{1, 2, 3, 4, 5, 6});
  attribute attr7(std::vector<double>{10.0, 20.0, 30.0, 40.0, 50.0, 60.0});
}

BOOST_AUTO_TEST_CASE(size_check)
{
  // std::cout << sizeof(attribute) << std::endl;
}

BOOST_AUTO_TEST_CASE(conversion_to_scalar)
{
  using conv = attribute_converter;

  std::string val_str = "1";
  double val_dbl = 2.0;
  std::int64_t val_int = 3;
  attribute_array<double> val_dbla{4};
  attribute_array<std::int64_t> val_inta{5};
  attribute_array<std::string> val_stra{"6"};

  optional<double> str2dbl = conv::convert_to<double>(val_str);
  optional<double> dbl2dbl = conv::convert_to<double>(val_dbl);
  optional<double> int2dbl = conv::convert_to<double>(val_int);
  optional<double> dbla2dbl = conv::convert_to<double>(val_dbla);
  optional<double> inta2dbl = conv::convert_to<double>(val_inta);
  optional<double> stra2dbl = conv::convert_to<double>(val_stra);

  BOOST_REQUIRE_EQUAL(str2dbl.get(), 1.0);
  BOOST_REQUIRE_EQUAL(dbl2dbl.get(), 2.0);
  BOOST_REQUIRE_EQUAL(int2dbl.get(), 3.0);
  BOOST_REQUIRE_EQUAL(dbla2dbl.get(), 4.0);
  BOOST_REQUIRE_EQUAL(inta2dbl.get(), 5.0);
  BOOST_REQUIRE_EQUAL(stra2dbl.get(), 6.0);

  optional<std::int64_t> str2int = conv::convert_to<std::int64_t>(val_str);
  optional<std::int64_t> dbl2int = conv::convert_to<std::int64_t>(val_dbl);
  optional<std::int64_t> int2int = conv::convert_to<std::int64_t>(val_int);
  optional<std::int64_t> dbla2int = conv::convert_to<std::int64_t>(val_dbla);
  optional<std::int64_t> inta2int = conv::convert_to<std::int64_t>(val_inta);
  optional<std::int64_t> stra2int = conv::convert_to<std::int64_t>(val_stra);

  BOOST_REQUIRE_EQUAL(str2int.get(), 1);
  BOOST_REQUIRE_EQUAL(dbl2int.get(), 2);
  BOOST_REQUIRE_EQUAL(int2int.get(), 3);
  BOOST_REQUIRE_EQUAL(dbla2int.get(), 4);
  BOOST_REQUIRE_EQUAL(inta2int.get(), 5);
  BOOST_REQUIRE_EQUAL(stra2int.get(), 6);

  optional<std::string> str2str = conv::convert_to<std::string>(val_str);
  optional<std::string> dbl2str = conv::convert_to<std::string>(val_dbl);
  optional<std::string> int2str = conv::convert_to<std::string>(val_int);
  optional<std::string> dbla2str = conv::convert_to<std::string>(val_dbla);
  optional<std::string> inta2str = conv::convert_to<std::string>(val_inta);
  optional<std::string> stra2str = conv::convert_to<std::string>(val_stra);

  BOOST_REQUIRE_EQUAL(str2str.get(), "1");
  BOOST_REQUIRE_EQUAL(dbl2str.get(), "2");
  BOOST_REQUIRE_EQUAL(int2str.get(), "3");
  BOOST_REQUIRE_EQUAL(dbla2str.get(), "4");
  BOOST_REQUIRE_EQUAL(inta2str.get(), "5");
  BOOST_REQUIRE_EQUAL(stra2str.get(), "6");
}

BOOST_AUTO_TEST_CASE(conversion_to_double_array)
{
  using conv = attribute_converter;

  std::string val_str = "1";
  double val_dbl = 2.0;
  std::int64_t val_int = 3;
  attribute_array<double> val_dbla(10, 4);
  attribute_array<std::int64_t> val_inta(10, 5);
  attribute_array<std::string> val_stra(10, "6.0");

  using dbla = attribute_array<double>;

  dbla str2dbla = conv::convert_to<dbla>(val_str).get();
  dbla dbl2dbla = conv::convert_to<dbla>(val_dbl).get();
  dbla int2dbla = conv::convert_to<dbla>(val_int).get();
  dbla dbla2dbla = conv::convert_to<dbla>(val_dbla).get();
  dbla inta2dbla = conv::convert_to<dbla>(val_inta).get();
  dbla stra2dbla = conv::convert_to<dbla>(val_stra).get();

  dbla dbla1{1.0}, dbla2{2.0}, dbla3{3.0};

  BOOST_REQUIRE_EQUAL_COLLECTIONS(str2dbla.begin(), str2dbla.end(),
                                  dbla1.begin(), dbla1.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbl2dbla.begin(), dbl2dbla.end(),
                                  dbla2.begin(), dbla2.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(int2dbla.begin(), int2dbla.end(),
                                  dbla3.begin(), dbla3.end());
  dbla dbla4(10, 4.0), dbla5(10, 5.0), dbla6(10, 6.0);

  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbla2dbla.begin(), dbla2dbla.end(),
                                  dbla4.begin(), dbla4.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(inta2dbla.begin(), inta2dbla.end(),
                                  dbla5.begin(), dbla5.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(stra2dbla.begin(), stra2dbla.end(),
                                  dbla6.begin(), dbla6.end());
}

BOOST_AUTO_TEST_CASE(conversion_to_int_array)
{
  using conv = attribute_converter;

  std::string val_str = "1";
  double val_dbl = 2.0;
  std::int64_t val_int = 3;
  attribute_array<double> val_dbla(10, 4);
  attribute_array<std::int64_t> val_inta(10, 5);
  attribute_array<std::string> val_stra(10, "6");

  using inta = attribute_array<std::int64_t>;

  inta str2stra = conv::convert_to<inta>(val_str).get();
  inta dbl2stra = conv::convert_to<inta>(val_dbl).get();
  inta int2stra = conv::convert_to<inta>(val_int).get();
  inta dbla2stra = conv::convert_to<inta>(val_dbla).get();
  inta inta2stra = conv::convert_to<inta>(val_inta).get();
  inta stra2stra = conv::convert_to<inta>(val_stra).get();

  inta stra1{1}, stra2{2}, stra3{3};

  BOOST_REQUIRE_EQUAL_COLLECTIONS(str2stra.begin(), str2stra.end(),
                                  stra1.begin(), stra1.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbl2stra.begin(), dbl2stra.end(),
                                  stra2.begin(), stra2.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(int2stra.begin(), int2stra.end(),
                                  stra3.begin(), stra3.end());

  inta stra4(10, 4), stra5(10, 5), stra6(10, 6);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbla2stra.begin(), dbla2stra.end(),
                                  stra4.begin(), stra4.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(inta2stra.begin(), inta2stra.end(),
                                  stra5.begin(), stra5.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(stra2stra.begin(), stra2stra.end(),
                                  stra6.begin(), stra6.end());
}

BOOST_AUTO_TEST_CASE(conversion_to_str_array)
{
  using conv = attribute_converter;

  std::string val_str = "1";
  double val_dbl = 2.0;
  std::int64_t val_int = 3;
  attribute_array<double> val_dbla(10, 4);
  attribute_array<std::int64_t> val_inta(10, 5);
  attribute_array<std::string> val_stra(10, "6");

  using stra = attribute_array<std::string>;

  stra str2stra = conv::convert_to<stra>(val_str).get();
  stra dbl2stra = conv::convert_to<stra>(val_dbl).get();
  stra int2stra = conv::convert_to<stra>(val_int).get();
  stra dbla2stra = conv::convert_to<stra>(val_dbla).get();
  stra inta2stra = conv::convert_to<stra>(val_inta).get();
  stra stra2stra = conv::convert_to<stra>(val_stra).get();

  stra stra1{"1"}, stra2{"2"}, stra3{"3"};
  stra stra4(10, "4"), stra5(10, "5"), stra6(10, "6");

  BOOST_REQUIRE_EQUAL_COLLECTIONS(str2stra.begin(), str2stra.end(),
                                  stra1.begin(), stra1.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbl2stra.begin(), dbl2stra.end(),
                                  stra2.begin(), stra2.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(int2stra.begin(), int2stra.end(),
                                  stra3.begin(), stra3.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbla2stra.begin(), dbla2stra.end(),
                                  stra4.begin(), stra4.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(inta2stra.begin(), inta2stra.end(),
                                  stra5.begin(), stra5.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(stra2stra.begin(), stra2stra.end(),
                                  stra6.begin(), stra6.end());
}

BOOST_AUTO_TEST_CASE(conversion_to_coord_double3)
{
  using conv = attribute_converter;

  attribute_array<double> val_dbla(3, 4);
  attribute_array<std::int64_t> val_inta(3, 5);
  attribute_array<std::string> val_stra(3, "6");

  using coords3d = coordinates<double, 3>;

  coords3d dbla2coord3d = conv::convert_to<coords3d>(val_dbla).get();
  coords3d inta2coord3d = conv::convert_to<coords3d>(val_inta).get();
  coords3d stra2coord3d = conv::convert_to<coords3d>(val_stra).get();

  coords3d coords4{4.0, 4.0, 4.0}, coords5{5.0, 5.0, 5.0},
      coords6{6.0, 6.0, 6.0};

  BOOST_REQUIRE_EQUAL_COLLECTIONS(dbla2coord3d.begin(), dbla2coord3d.end(),
                                  coords4.begin(), coords4.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(inta2coord3d.begin(), inta2coord3d.end(),
                                  coords5.begin(), coords5.end());
  BOOST_REQUIRE_EQUAL_COLLECTIONS(stra2coord3d.begin(), stra2coord3d.end(),
                                  coords6.begin(), coords6.end());
}

BOOST_AUTO_TEST_CASE(hash)
{
  std::unordered_set<attribute> set;
  set.emplace("string");
}

BOOST_AUTO_TEST_CASE(reference_getters)
{
  attribute attr0{1.0, 2.0};
  BOOST_REQUIRE_EQUAL(attr0.get_real_ref(0), 1.0);
  BOOST_REQUIRE_EQUAL(attr0.get_real_ref(1), 2.0);

  attribute const &cattr0(attr0);
  BOOST_REQUIRE_EQUAL(cattr0.get_real_ref(0), 1.0);
  BOOST_REQUIRE_EQUAL(cattr0.get_real_ref(1), 2.0);
}

BOOST_AUTO_TEST_SUITE_END()
