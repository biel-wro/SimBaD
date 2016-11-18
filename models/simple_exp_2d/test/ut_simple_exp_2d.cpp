#include <iostream>

#include <boost/test/auto_unit_test.hpp>

#include "interface/event.hpp"
#include "event_kind.hpp"
#include "interface/model.hpp"
#include "property_tree.hpp"
#include "simple_exp_2d_factory.hpp"

BOOST_AUTO_TEST_CASE(config_run)
{
  simbad::models::simple_exp_2d::simple_exp_2d_factory factory;

  simbad::core::property_tree mp;

  mp.put("dispersion.sigma", 1);

  mp.put("interaction.sigma", 1);
  mp.put("interaction.gamma", 2);
  mp.put("interaction.tolerance", 1e-6);

  mp.put("saturation.sigma", 10);
  mp.put("saturation.gamma", 2);
  mp.put("saturation.scale", 1);

  mp.put("death.rate", 0.1);

  mp.put("space.tile.size", 3);
  mp.put("seed", 1);

  std::unique_ptr<simbad::core::model> up_model = factory.create_instance(mp);

  simbad::core::model *p_model = up_model.get();
  auto visitor = [](simbad::core::event const &e) {

    std::cout << "t = " << e.time();
    for(size_t p = 0; p < e.npartials(); ++p)
    {
      std::cout << ", " << e.partial_type(p) << " (" << e.coord(p, 0) << ", "
                << e.coord(p, 1) << ")";
    }
    std::cout<<std::endl;
  };

  p_model->generate_events(visitor, 100);
}
