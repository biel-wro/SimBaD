#ifndef SIMPLE_EXP_2D_PARTICLE_HPP
#define SIMPLE_EXP_2D_PARTICLE_HPP

#include "config.hpp"
#include "coordinates/coordinates.hpp"
#include "density_accumulator.hpp"
#include "object_tracking/handle_trackee.hpp"
#include "legacy/simple/simple_event_queue.hpp"
#include "simple_exp_2d_fwd.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

class particle : public simbad::core::handle_trackee<
                     simbad::core::simple_event_queue::handle_type, particle>
{
public:
  using self_base_type = simbad::core::handle_trackee<
      simbad::core::simple_event_queue::handle_type, particle>;
  using queue_handle_type = simbad::core::simple_event_queue::handle_type;
  using space_coords = config::space_coords;

  explicit particle(queue_handle_type h = queue_handle_type(nullptr));
  explicit particle(space_coords sc,
                    queue_handle_type h = queue_handle_type(nullptr));
  void set_handle(queue_handle_type handle);
  queue_handle_type get_handle() const;

  space_coords &coords();
  const space_coords &coords() const;

  density_accumulator &get_density_accumulator();
  density_accumulator const &get_density_accumulator() const;

private:
  space_coords m_coords;
  density_accumulator m_density_accumulator;
};
END_NAMESPACE_SIMPLE_EXP_2D
#endif // PARTILCLE_HPP
