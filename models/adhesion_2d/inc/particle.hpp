#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "adhesion_2d_fwd.hpp"

#include "computational/templates/accumulator.hpp"
#include "coordinates/coordinates.hpp"
#include "interface/particle.hpp"
#include <limits>

BEGIN_NAMESPACE_ADHESION_2D

class cell
{
public:
  static constexpr size_t dimension = 2;
  using coord_scalar = double;
  using position_type = simbad::core::coordinates<coord_scalar, dimension>;
  using time_type = double;
  using velocity_type = simbad::core::coordinates<double, dimension>;
  using force_type = simbad::core::coordinates<double, dimension>;
  using pressure_type = float;

  explicit cell(position_type pos = position_type(),
                velocity_type v = velocity_type(0),
                time_type et = std::numeric_limits<time_type>::infinity(),
                time_type dt = std::numeric_limits<time_type>::infinity());
  ~cell();

  position_type const &position() const;
  position_type &position();
  time_type next_jump_time() const;
  time_type &next_jump_time();
  time_type prev_jump_time() const;
  time_type &prev_jump_time();
  time_type delta_time() const;

  velocity_type const &velocity() const;
  velocity_type &velocity();
  force_type const &force() const;
  force_type &force();
  pressure_type pressure() const;
  pressure_type &pressure();
private:
  position_type m_position;
  velocity_type m_velocity;
  force_type m_force;
  time_type m_next_time;
  time_type m_prev_time;
  time_type m_delta_time;
  pressure_type m_pressure;
};

class particle_view : public simbad::core::particle
{
public:
  using orig = simbad::models::adhesion_2d::cell;
  explicit particle_view(orig const *ptr = nullptr);
  void set_orig(const orig *ptr);
  //std::size_t dimension() const override;
  double coord(std::size_t d) const override;
  //simbad::core::attribute
  //extra_attribute(std::size_t) const override;

private:
  orig const *m_ptr;
};

END_NAMESPACE_ADHESION_2D
#endif // PARTICLE_HPP
