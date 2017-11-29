#include <algorithm>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>

//#include "core_fwd.hpp"
#include "builtin_models.hpp"

#include "interface/model_register.hpp"

using namespace simbad::core;
using namespace simbad::models::common;

BOOST_AUTO_TEST_CASE(builtin_nonempty)
{
  simbad::core::model_register const &reg = get_builtin_models();

  auto names = reg.get_model_names();

  BOOST_REQUIRE_GT(std::distance(names.begin(), names.end()), 0);
}
