#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "adhesion_2d_fwd.hpp"

#include "accumulator.hpp"
#include "coordinates/coordinates.hpp"
#include "interface/particle.hpp"
#include <limits>

BEGIN_NAMESPACE_ADHESION_2D

class particle
{
public:
  static constexpr size_t dimension = 2;
  using coord_scalar = double;
  using position_type = simbad::core::coordinates<coord_scalar, dimension>;
  using time_type = double;
  using velocity_type = simbad::core::coordinates<double, dimension>;
  using acceleration_type = simbad::core::coordinates<double, dimension>;

  explicit particle(position_type pos = position_type(),
                    velocity_type v = velocity_type(0),
                    acceleration_type acc = acceleration_type(0),
                    time_type et = std::numeric_limits<time_type>::infinity(),
                    time_type dt = std::numeric_limits<time_type>::infinity());
  ~particle();

  position_type const &position() const;
  position_type &position();

  time_type event_time() const;
  time_type &event_time();

  time_type delta_time() const;
  time_type &delta_time();

  velocity_type const &velocity() const;
  velocity_type &velocity();

  acceleration_type const &acceleration() const;
  acceleration_type &acceleration();

private:
  position_type m_position;
  time_type m_event_time;
  time_type m_delta_time;
  velocity_type m_velocity;
  acceleration_type m_acceleration;
};

class particle_view : public simbad::core::particle
{
public:
  using orig = simbad::models::adhesion_2d::particle;
  explicit particle_view(orig const *ptr=nullptr);
  void set_orig(const orig *ptr);
  std::size_t dimension() const override;
  double coord(std::size_t d) const override;

private:
  orig const *m_ptr;
};

END_NAMESPACE_ADHESION_2D
#endif // PARTICLE_HPP
