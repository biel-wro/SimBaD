#include <iostream>

#include <boost/intrusive/unordered_set_hook.hpp>
#include <boost/test/auto_unit_test.hpp>


#include "coordinates/coordinates.hpp"
#include "scale_coord_tiler.hpp"
#include "spacetime/cartesian_spacetime.hpp"

using namespace simbad::core;
namespace
{
using coords = coordinates<float, 3>;

struct Particle
{
  Particle(float t, float x, float y, float z) : t(t), pos{x, y, z} {}
  float t;
  coords pos;
};
struct Ordering
{
  bool operator()(Particle const &lhs, Particle const &rhs) const
  {
    return rhs.t < lhs.t;
  }
};
struct CoordGetter
{
  coords operator()(const Particle &p) const { return p.pos; }
};

struct ParticleTraits
{
  using particle = Particle;
  using ordering = Ordering;
  using coord_scalar = float;
  using coord_vector = coords;
  using coord_getter = CoordGetter;
  using dimension_type = std::size_t;
  constexpr static dimension_type dimension = 3;
};

using CartesianSpacetime = cartesian_spacetime<ParticleTraits>;
}

BOOST_AUTO_TEST_SUITE(cartesian_spacetime)

BOOST_AUTO_TEST_CASE(one_particle)
{
  CartesianSpacetime cs;
  cs.insert(Particle{1, 10.1f, 20.2f, 30.3f});

  int nvisited = 0;
  cs.visit([&nvisited](Particle const &p) {
    nvisited++;
    BOOST_REQUIRE_EQUAL(p.t, 1);
    BOOST_REQUIRE_EQUAL(p.pos[0], 10.1f);
    BOOST_REQUIRE_EQUAL(p.pos[1], 20.2f);
    BOOST_REQUIRE_EQUAL(p.pos[2], 30.3f);
  });
  BOOST_REQUIRE_EQUAL(nvisited, 1);
  BOOST_REQUIRE_EQUAL(cs.size(), 1);

  cs.pop();
  BOOST_REQUIRE_EQUAL(cs.size(), 0);
}

BOOST_AUTO_TEST_CASE(visit_ball)
{
  CartesianSpacetime cs;
  Particle p(1, -10.f, -10.f, 10.f);
  cs.insert(p);

  int nvisited;
  coords origin{0, 0, 0};

  nvisited = 0;
  cs.visit_ball(origin, 17.3, [origin, &nvisited](Particle const &p) {
    nvisited++;
  });
  BOOST_REQUIRE_EQUAL(nvisited, 0);

  nvisited = 0;
  cs.visit_ball(origin, 17.4, [origin, &nvisited](Particle const &p) {
    nvisited++;
  });
  BOOST_REQUIRE_EQUAL(nvisited, 1);
}
BOOST_AUTO_TEST_SUITE_END()
