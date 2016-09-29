#include "kernel.hpp"
#include <boost/test/auto_unit_test.hpp>

using simbad::models::simple_exp_2d::kernel;

BOOST_AUTO_TEST_SUITE(gamma_kernel)

BOOST_AUTO_TEST_CASE(param_get)
{
  float gamma = 1.23;
  float sigma = 3.4523;
  float tolerance = 0.001;

  kernel k(sigma, gamma, tolerance);

  BOOST_REQUIRE_CLOSE(k.get_gamma(), gamma, 0.001);
  BOOST_REQUIRE_CLOSE(k.get_sigma(), sigma, 0.001);
}

BOOST_AUTO_TEST_CASE(range)
{
  float gamma = 1.23;
  float sigma = 3.4523;
  float tolerance = 0.001;

  kernel k(sigma, gamma, tolerance);

  float range_squared = k.range_squared();
  BOOST_CHECK_GE(range_squared, sigma);

  BOOST_REQUIRE_CLOSE(k(range_squared), tolerance, 0.001);
}

BOOST_AUTO_TEST_CASE(gamma_infinite)
{
  float gamma = std::numeric_limits<float>::infinity();
  float sigma = 7.68;
  float tolerance = 0.001;

  kernel k(sigma, gamma, tolerance);

  BOOST_CHECK(k.get_gamma() == std::numeric_limits<float>::infinity() );
  BOOST_CHECK_CLOSE(k.get_sigma(), sigma, 0.001);

  float range_squared = k.range_squared();
  BOOST_CHECK_GE(range_squared, sigma);

  BOOST_CHECK_EQUAL(k(range_squared - 0.01), 1);
  BOOST_CHECK_EQUAL(k(range_squared + 0.01), 0);
}

BOOST_AUTO_TEST_CASE(gamma_zero)
{
  float gamma = 0;
  float sigma = 7.68;
  float tolerance = 0.001;

  kernel k(sigma, gamma, tolerance);

  BOOST_CHECK_EQUAL(k.get_gamma(),  0 );
  //BOOST_CHECK_CLOSE(k.get_sigma(), sigma, 0.001);

  float range_squared = k.range_squared();
  BOOST_CHECK_EQUAL(range_squared, 0);

  BOOST_CHECK_EQUAL(k(range_squared - 0.01), 1);
  BOOST_CHECK_EQUAL(k(range_squared + 0.01), 0);
}

BOOST_AUTO_TEST_CASE(sigma_zero)
{
  float gamma = 5.0;
  float sigma = 0;
  float tolerance = 0.001;

  kernel k(sigma, gamma, tolerance);

  BOOST_CHECK_CLOSE(k.get_gamma(), gamma, 0.001);
  BOOST_CHECK_CLOSE(k.get_sigma(), sigma, 0.001);

  float range_squared = k.range_squared();
  BOOST_CHECK_GE(range_squared, sigma);

  BOOST_CHECK_EQUAL(k(0), 1);
  BOOST_CHECK_EQUAL(k(0.001), 0);
}

BOOST_AUTO_TEST_CASE(sigma_infinite)
{
  float gamma = 5.0;
  float sigma = std::numeric_limits<float>::infinity();
  float tolerance = 0.001;

  kernel k(sigma, gamma, tolerance);

  BOOST_CHECK_CLOSE(k.get_gamma(), gamma, 0.001);
  BOOST_CHECK_EQUAL(k.get_sigma(), sigma);

  float range_squared = k.range_squared();
  BOOST_CHECK_GE(range_squared, sigma);

  BOOST_CHECK_EQUAL(k(0), 1);
  BOOST_CHECK_EQUAL(k(0.001), 1);
}

BOOST_AUTO_TEST_SUITE_END()
