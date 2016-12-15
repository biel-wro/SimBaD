#include <boost/test/auto_unit_test.hpp>

#include "computational/distributions/power_law_distribution.hpp"

#include <random>

using namespace simbad::core;
using namespace boost::math;

BOOST_AUTO_TEST_CASE(power_law_instantiation)
{
    power_law_distribution<double> d;
}

BOOST_AUTO_TEST_CASE(power_law_construction)
{
    using pwr = power_law_distribution<double>;
    using prt = pareto_distribution<double>;

    pwr d1;
    prt d2;
    double x = 3.14;

    double q = 0.11;

    BOOST_CHECK_CLOSE(mean(pwr()), mean(prt()), 10);
    BOOST_CHECK_CLOSE(median(pwr()), median(prt()), 10);

    BOOST_CHECK_CLOSE(variance(pwr(6, 6)), variance(prt(6, 5)), 10);
    BOOST_CHECK_CLOSE(standard_deviation(pwr(6, 6)),
                      standard_deviation(prt(6, 5)), 10);

    BOOST_CHECK_CLOSE(pdf(pwr(), x), pdf(prt(), x), 10);
    BOOST_CHECK_CLOSE(cdf(pwr(), x), cdf(prt(), x), 10);
    BOOST_CHECK_CLOSE(quantile(pwr(), q), quantile(prt(), q), 10);
    BOOST_CHECK_CLOSE(hazard(pwr(), x), hazard(prt(), x), 10);
    BOOST_CHECK_CLOSE(chf(pwr(), x), chf(prt(), x), 10);
}
