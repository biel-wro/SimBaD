#include <boost/test/auto_unit_test.hpp>
#include <iostream>

#include "core_fwd.hpp"
#include "model_register.hpp"

#include "builtin_models.hpp"



using namespace simbad::core;
using namespace simbad::models::common;

BOOST_AUTO_TEST_CASE(builtin_nonempty)
{
    simbad::core::model_register const &reg = get_buiildin_models();

    for( std::string name: reg.get_model_names() )
    {
        std::cout<< name<< std::endl;
    }
}
