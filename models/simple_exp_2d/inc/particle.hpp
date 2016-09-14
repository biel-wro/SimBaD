#ifndef SIMPLE_EXP_2D_PARTICLE_HPP
#define SIMPLE_EXP_2D_PARTICLE_HPP

#include "config.hpp"
#include "coordinates.hpp"
#include "handle_trackee.hpp"
#include "simple_event_queue.hpp"
#include "simple_exp_2d_fwd.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

struct particle : public simbad::core::handle_trackee<
                      simbad::core::simple_event_queue::handle_type, particle>
{
public:
  using self_base_type = simbad::core::handle_trackee<
      simbad::core::simple_event_queue::handle_type, particle>;
  using queue_handle_type = simbad::core::simple_event_queue::handle_type;
  using space_coords = config::space_coords;

  explicit particle(queue_handle_type h = queue_handle_type(nullptr));
  explicit particle(space_coords sc, queue_handle_type h = queue_handle_type(nullptr));
  void set_handle(queue_handle_type handle);
  queue_handle_type get_handle() const;

  space_coords &coords();
  const space_coords &coords() const;

private:
  space_coords m_coords;
};
END_NAMESPACE_SIMPLE_EXP_2D
#endif // PARTILCLE_HPP
