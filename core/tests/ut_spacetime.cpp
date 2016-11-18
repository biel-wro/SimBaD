
#include <boost/intrusive/unordered_set_hook.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "coordinates/coordinates.hpp"
#include "coordinates/scale_coord_tiler.hpp"
#include "spacetime/spacetime.hpp"

using namespace simbad::core;
namespace
{
struct Particle
{
  explicit Particle(float t = 0.f, float x = 0.f, float y = 0.f, float z = 0.f)
      : t(t), x(x), y(y), z(z)
  {
  }
  float t, x, y, z;
};
struct ParticleOrdering
{
  bool operator()(Particle const &p1, Particle const &p2) const
  {
    return p1.t > p2.t;
  }
};

using TileKey = coordinates<int, 3>;
struct Tiler
{
  TileKey operator()(Particle const &p) const
  {
    return m_ctiler(coordinates<float, 3>{p.x, p.y, p.z});
  }
  scale_coord_tiler<float, int, 3> m_ctiler;
};

struct SpaceTimeTraits
{
  using particle_type = Particle;
  using particle_ordering = ParticleOrdering;

  using tiler = Tiler;
  using tile_key = TileKey;
  using tile_key_equal = std::equal_to<tile_key>;
  using tile_key_hash = coord_hasher;
};

using Spacetime = spacetime<SpaceTimeTraits>;
}
BOOST_AUTO_TEST_SUITE(ut_spacetime)

BOOST_AUTO_TEST_CASE(single_emplacement)
{
  Spacetime st;
  Tiler tiler;

  Particle p{1.f, 2.f, 3.f, 4.f};
  TileKey tk = tiler(p);
  st.emplace(tk, p);
  st.visit([p](Particle const &p2) {
    BOOST_REQUIRE_EQUAL(p2.t, p.t);
    BOOST_REQUIRE_EQUAL(p2.x, p.x);
    BOOST_REQUIRE_EQUAL(p2.y, p.y);
    BOOST_REQUIRE_EQUAL(p2.z, p.z);
  });
  BOOST_REQUIRE_EQUAL(st.size(), 1);
  BOOST_REQUIRE(!st.empty());

  st.pop();
  BOOST_REQUIRE_EQUAL(st.size(), 0 );
  BOOST_REQUIRE(st.empty());
}
BOOST_AUTO_TEST_SUITE_END()
