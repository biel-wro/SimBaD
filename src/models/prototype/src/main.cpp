#include "board.hpp"
#include "simple_particle.hpp"

#include <cmath>
#include <iostream>
#include <random>

using Point = simbad::core::simple_particle<2, float>;
using Board = simbad::core::board<Point, 2>;
using BoardCoordinates = simbad::core::board_coordinates<int, 2>;

using Event = simbad::core::simple_event;
using EventHandle = simbad::core::simple_event_handle;
using EventQueue = simbad::core::simple_event_queue;

using ConstNeighbourhood = Board::const_box_region;
using Neighbourhood = Board::box_region;

struct TileMapper
{
  TileMapper(float tile_size = 1.0f) : tile_size(tile_size) {}

  BoardCoordinates operator()(Point const &p) const
  {
    return operator()(p.get_coordinate<0>(), p.get_coordinate<1>());
  }

  BoardCoordinates operator()(double x, double y) const
  {
    BoardCoordinates bc;
    float bcx = std::floor(x / tile_size);
    float bcy = std::floor(y / tile_size);

    bc[0] = static_cast<int>(bcx);
    bc[1] = static_cast<int>(bcy);

    return bc;
  }

  float tile_size;
};
struct DeathRateAccumulator
{
  DeathRateAccumulator() : acc(0.0) {}

  void accumulate(Point const &p, Point const &other)
  {
    (void)p;
    (void)other;
  }

  double get_range() const { return 0.0; }
  double get_intensity() const { return 0.1; }

  double acc;
};

struct BirthRateAccumulator
{
  BirthRateAccumulator() : range(1.0), cnt(100) {}
  void accumulate(Point const &p, Point const &other)
  {
    double dx = p.get_coordinate<0>() - other.get_coordinate<0>();
    double dy = p.get_coordinate<1>() - other.get_coordinate<1>();
    if(std::hypot(dx, dy) < get_range())
      cnt++;
  }

  double get_intensity() const { return cnt < 0 ? 0 : 1; }
  double get_range() const { return range; }

  double range;
  int32_t cnt;
};

Board initial_configuration()
{
  Board b;
  Point p;
  p.set_coordinate<0>(0.0f);
  p.set_coordinate<1>(0.0f);
  BoardCoordinates bc = TileMapper()(p);
  b.emplace(bc, std::move(p));
  return b;
}

Event compute_event(std::mt19937_64 &rnd, Point const &p, Board const &b)
{
  DeathRateAccumulator death_acc;
  double death_intrange = death_acc.get_range();

  BirthRateAccumulator birth_acc;
  double birth_intrange = birth_acc.get_range();

  double range = std::max(death_intrange, birth_intrange);
  double centerx = p.get_coordinate<0>();
  double centery = p.get_coordinate<1>();

  BoardCoordinates min_coords = TileMapper()(centerx - range, centery - range);
  BoardCoordinates max_coords = TileMapper()(centerx + range, centery + range);

  ConstNeighbourhood neighbourhood = b.get_box(min_coords, max_coords);

  for(Point const &other : neighbourhood)
  {
    death_acc.accumulate(p, other);
    birth_acc.accumulate(p, other);
  }

  double death_intensity = death_acc.get_intensity();
  double birth_intensity = birth_acc.get_intensity();

  double death_time = std::exponential_distribution<>(death_intensity)(rnd);
  double birth_time = std::exponential_distribution<>(birth_intensity)(rnd);

  Event event;
  event.set_event_type(death_time < birth_time ? 0 : 1);
  event.set_time(static_cast<float>(std::min(death_time, birth_time)));

  return event;
}
EventQueue initial_event_queue(std::mt19937_64 &rnd, Board &board)
{
  EventQueue eq;

  for(Point &p : board)
  {
    Event e = compute_event(rnd, p, board);
    e.set_particle_ptr(&p);
    EventHandle h = eq.emplace(std::move(e));
    p.set_handle(h);
  }

  return eq;
}

void update_neighbourhood_events(std::mt19937_64 &rnd, EventQueue &eq,
                                 double cx, double cy, double r,
                                 Board const &board, double time_offset)
{
  BoardCoordinates min_coords = TileMapper()(cx - r, cy - r);
  BoardCoordinates max_coords = TileMapper()(cx + r, cy + r);

  ConstNeighbourhood neighbourhood = board.get_box(min_coords, max_coords);

  for(Point const &point : neighbourhood)
  {
    Event event = compute_event(rnd, point, board);
    event.set_time(event.get_time() + time_offset);
    EventHandle h = point.get_handle();
    *h = std::move(event);
    eq.update_lazy(h);
  }
}

void execute_death(Board &board, EventQueue &eq)
{
  Point const &point = *static_cast<Point const *>(eq.top().get_particle_ptr());
  BoardCoordinates bc = TileMapper()(point);
  board.remove(bc, const_cast<Point &>(point));
  eq.pop();
}

Point &execute_birth(std::mt19937_64 &rnd, Board &board, EventQueue &eq)
{
  Point const &point = *static_cast<Point const *>(eq.top().get_particle_ptr());
  double parent_x = point.get_coordinate<0>();
  double parent_y = point.get_coordinate<1>();

  EventHandle h = eq.emplace(Event());

  double offspring_x = std::normal_distribution<>(parent_x, 1)(rnd);
  double offspring_y = std::normal_distribution<>(parent_y, 1)(rnd);

  BoardCoordinates bc = TileMapper()(offspring_x, offspring_y);
  Point &offspring = *board.emplace(bc, h);
  offspring.set_coordinate<0>(offspring_x);
  offspring.set_coordinate<1>(offspring_y);

  return offspring;
}

void execute_event(double &t, std::mt19937_64 &rnd, Board &board,
                   EventQueue &eq)
{
  Point const &point = *static_cast<Point const *>(eq.top().get_particle_ptr());
  double cx = point.get_coordinate<0>();
  double cy = point.get_coordinate<1>();
  double r = 1;
  t = eq.top().get_time();

  if(eq.top().get_event_type() == 0)
    execute_death(board, eq);
  else
  {
    Point &offspring = execute_birth(rnd, board, eq);
    update_neighbourhood_events(rnd, eq, offspring.get_coordinate<0>(),
                                offspring.get_coordinate<1>(), 1, board, t);
  }

  update_neighbourhood_events(rnd, eq, cx, cy, r, board, t);
}

int main()
{
  size_t niters = 1;

  std::mt19937_64 random_engine;
  Board board = initial_configuration();
  EventQueue eq = initial_event_queue(random_engine, board);

  double time = 0.0;
  for(size_t iter = 0; iter < niters; ++iter)
  {
    execute_event(time, random_engine, board, eq);
  }

  for(Point const &p : board)
  {
    std::cout << p.get_coordinate<0>() << " " << p.get_coordinate<1>()
              << std::endl;
  }

  return 0;
}
