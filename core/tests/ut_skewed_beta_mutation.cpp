#include <boost/test/auto_unit_test.hpp>

#include "skewed_beta_mutation.hpp"
#include <boost/math/distributions/beta.hpp>
#include <iostream>
using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_skewed_beta_mutation)

BOOST_AUTO_TEST_CASE(beta_finding)
{
  constexpr double alpha = 2;
  constexpr double improvement_prob = 0.01;
  constexpr double parent_val = 0.1;
  skewed_beta_mutator<double> mut(alpha, improvement_prob);
  double beta = mut.find_beta(parent_val);

  boost::math::beta_distribution<double> dist(alpha, beta);
  double cdf = boost::math::cdf(dist, parent_val);

  BOOST_REQUIRE_CLOSE(1 - cdf, improvement_prob, 0.00001);
}

BOOST_AUTO_TEST_CASE(sampling)
{
  constexpr double alpha = 2;
  constexpr double improvement_prob = 0.01;
  constexpr double parent_val = 0.2;
  skewed_beta_mutator<double> mut(alpha, improvement_prob);
  std::mt19937_64 rng;

  //test is random, but probability of failing is low
  for(int i = 0; i < 100; ++i)
  {
    double child_val = mut(parent_val, rng);
    BOOST_TEST_CHECKPOINT("i="<<i);
    BOOST_REQUIRE_LE( child_val, parent_val);
  }
}

BOOST_AUTO_TEST_SUITE_END()
