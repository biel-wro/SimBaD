#include <boost/test/auto_unit_test.hpp>

#include "computational/mutations/templ/skewed_polynomial_mutator.hpp"
#include <boost/math/distributions/beta.hpp>
#include <iostream>
using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_skewed_poly_mutation)

BOOST_AUTO_TEST_CASE(sampling_skewed_poly)
{
  constexpr double improvement_prob = 0.001;
  constexpr double increase_power = 2.0;
  constexpr double decrease_power = 1.0;

  skewed_polynomial_mutator<double> mut(improvement_prob, increase_power,
                                        decrease_power);
  std::mt19937_64 rng;

  double parent_val = 0.7;
  // test is random, but probability of failing is low
  for(int i = 0; i < 100; ++i)
  {
    double child_val = mut(parent_val, rng);
    // std::cout << child_val << std::endl;
    BOOST_TEST_CHECKPOINT("i=" << i);
    BOOST_REQUIRE_LE(child_val, parent_val);
  }
}

BOOST_AUTO_TEST_CASE(sampling2)
{
  constexpr std::size_t SAMPLE_SIZE = 1000;

  constexpr double improvement_prob = 0.1;
  constexpr double increase_power = 100.0;
  constexpr double decrease_power = 1.0;
  skewed_polynomial_mutator<double> mut(improvement_prob, increase_power,
                                        decrease_power);
  std::mt19937_64 rng;

  double parent_val = 0.1;
  std::vector<std::size_t> histogram(100);
  for(int i = 0; i < SAMPLE_SIZE; ++i)
  {
    double child_val = mut(parent_val, rng);
    histogram[child_val * histogram.size()]++;
  }
/*
  for(std::size_t i = 0; i < histogram.size(); ++i)
    std::cout << double(i) / histogram.size() << "\t" << histogram[i]
              << std::endl;*/
}

BOOST_AUTO_TEST_SUITE_END()
