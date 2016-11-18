#include <random>

#include <boost/intrusive/unordered_set_hook.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "coordinates/coordinates.hpp"
#include "spacetime/ordered_board.hpp"
#include "spacetime/queued_set.hpp"

using namespace simbad::core;

namespace
{
struct Particle
{
  Particle(float x, float y, float t) : x(x), y(y), t(t) {}
  float x, y;
  float t;
};
struct ParticleOrder
{
  bool operator()(Particle const &p1, Particle const &p2) const
  {
    return p1.t > p2.t;
  }
};

using OrderedHashSet = queued_set<Particle>;
using BoardCoords = coordinates<int, 2>;

struct OrderedBoardTraits
{
  using key_type = BoardCoords;
  using data_type = OrderedHashSet;

  using key_hash_pred = coord_hasher;
  using key_equal_pred = std::equal_to<key_type>;
  using data_order_pred = queued_set_compare<ParticleOrder>;
  using rehash_watchdog = auto_rehash<float>;
};

using OrderedBoard = ordered_board<OrderedBoardTraits>;
using DirtyHandle = typename OrderedBoard::dirty_handle_type;
using ConstHandle = OrderedBoard::const_handle_type;
}
BOOST_AUTO_TEST_SUITE(ut_ordered_board)

BOOST_AUTO_TEST_CASE(empty_board)
{
  OrderedBoard board;
  BOOST_REQUIRE(board.empty());
}

BOOST_AUTO_TEST_CASE(single_empty_tile)
{
  OrderedBoard board;
  board.emplace(BoardCoords{0,0});

  BOOST_REQUIRE(!board.empty());
  BOOST_REQUIRE_EQUAL(board.size(), 1);
}

BOOST_AUTO_TEST_CASE(one_nonempty_tile)
{
  OrderedBoard board;
  constexpr size_t NTILES = 10;

  for(size_t i = 0; i < NTILES; ++i)
    board.emplace(BoardCoords{int(i), int(i)});

  OrderedHashSet &tile = *board.find_dirty(BoardCoords{6, 6});
  tile.emplace(ParticleOrder(), 0.f, 0.f, 7.f);
  board.repair_order();

  BOOST_REQUIRE(!board.empty());
  BOOST_REQUIRE_EQUAL(board.size(), NTILES);
  BOOST_REQUIRE_EQUAL(&board.top(), &tile);
}

BOOST_AUTO_TEST_CASE(two_nonempty_tiles)
{

  BoardCoords coords1 = {1, 2};
  OrderedHashSet tile1;
  tile1.emplace(ParticleOrder(), Particle(1.5, 2.5, 20));

  BoardCoords coords2 = {3, 4};
  OrderedHashSet tile2;
  tile2.emplace(ParticleOrder(), Particle(3.5, 4.5, 10));

  OrderedBoard board;
  board.emplace(coords1, std::move(tile1));
  board.emplace(coords2, std::move(tile2));

  BOOST_REQUIRE(!board.empty());
  BOOST_REQUIRE_EQUAL(board.size(), 2);
  BOOST_REQUIRE_EQUAL(board.top().top().t, 10);
}

BOOST_AUTO_TEST_CASE(fill_and_empty_tiles)
{
  constexpr size_t NEVENTS = 10;
  OrderedBoard board;
  ParticleOrder particle_order;

  board.emplace(BoardCoords{0, 0});
  board.emplace(BoardCoords{0, 1});
  board.emplace(BoardCoords{1, 0});
  board.emplace(BoardCoords{1, 1});

  std::mt19937_64 rng;
  std::uniform_real_distribution<float> rnd(0, 2);

  std::vector<Particle> particles_in;
  particles_in.reserve(NEVENTS);

  for(size_t i = 0; i < NEVENTS; ++i)
  {
    float x = rnd(rng), y = rnd(rng);
    float t = rnd(rng);
    particles_in.emplace_back(x, y, t);

    BoardCoords tile_coords = {int(x), int(y)};

    DirtyHandle tile_handle = board.find_dirty(tile_coords);
    BOOST_REQUIRE(tile_handle);

    OrderedHashSet &tile = *tile_handle;
    tile.emplace(particle_order, x, y, t);
    board.repair_order(tile_handle);
  }

  std::vector<Particle> particles_out;
  particles_out.reserve(NEVENTS);

  for(size_t i = 0; i < NEVENTS; ++i)
  {
    OrderedHashSet &top_set = board.top_dirty();
    BOOST_REQUIRE(!top_set.empty());
    Particle &top_particle = top_set.top_dirty();
    particles_out.push_back(top_particle);
    top_set.pop_dirty();
    top_set.repair_order(particle_order);
    board.repair_order(); //(top_set);
  }

  BOOST_REQUIRE(std::is_sorted(particles_out.rbegin(), particles_out.rend(),
                               particle_order));
}

BOOST_AUTO_TEST_SUITE_END()
