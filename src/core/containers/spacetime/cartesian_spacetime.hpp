#ifndef CARTESIAN_SPACETIME_HPP
#define CARTESIAN_SPACETIME_HPP
#include "cartesian_spacetime_decl.hpp"
#include "containers/spacetime/spacetime.hpp"
#include "coordinates/coord_generators.hpp"
BEGIN_NAMESPACE_CORE

template <class Traits>
cartesian_spacetime<Traits>::cartesian_spacetime(coord_scalar tile_size,
                                                 ordering ord, coord_getter cg)
    : super(ord), m_tiler(tile_size), m_coord_getter(cg)
{
}

template <class Traits>
template <class... Args>
typename cartesian_spacetime<Traits>::const_handle_type
cartesian_spacetime<Traits>::emplace(coord_vector const &cv, Args... args)
{
  return super::emplace(m_tiler(cv), std::forward<Args>(args)...);
}

template <class Traits>
template <class... Args>
typename cartesian_spacetime<Traits>::dirty_handle_type
cartesian_spacetime<Traits>::emplace_dirty(coord_vector const &cv, Args... args)
{
  return super::emplace_dirty(m_tiler(cv), std::forward<Args>(args)...);
}

template <class Traits>
template <class P>
typename cartesian_spacetime<Traits>::const_handle_type
cartesian_spacetime<Traits>::insert(P &&p)
{
  return emplace(m_coord_getter(p), std::forward<P>(p));
}
template <class Traits>
template <class P>
typename cartesian_spacetime<Traits>::dirty_handle_type
cartesian_spacetime<Traits>::insert_dirty(P &&p)
{
  return emplace_dirty(m_coord_getter(p), std::forward<P>(p));
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_top_guarded(Visitor v)
{
  typename super::dirty_tile_handle th = super::board().first_dirty();
  typename super::dirty_subset_handle sh = th->first_dirty();

  assert(m_tiler(m_coord_getter(*sh)) == th.get_key());
  v(*sh);
  tile_coord_vector tile_vec = m_tiler(m_coord_getter(*sh));
  if(tile_vec == th.get_key())
  {
    super::repair_order(dirty_handle_type(th, sh));
    return;
  }

  particle p = super::pop_value();
  insert(std::move(p));
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_ball(coord_vector center_,
                                             coord_scalar r, Visitor v) const
{
  generic_coords c = generic_coords::from_indexed(center_);
  coord_generator_box<tile_coord_vector> tile_generator(m_tiler(c - r),
                                                        m_tiler(c + r));
  super::visit_region(tile_generator, [this, v, &c, &r](particle const &p) {
    generic_coords pc = generic_coords::from_indexed(m_coord_getter(p));

    if(c.distance_to(pc) <= r)
      v(p);
  });
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_ball_dirty(coord_vector center_,
                                                   coord_scalar r, Visitor v)
{
  generic_coords c = generic_coords::from_indexed(center_);
  coord_generator_box<tile_coord_vector> tile_generator(m_tiler(c - r),
                                                        m_tiler(c + r));
  super::visit_region_dirty(tile_generator, [this, v, &c, &r](particle &p) {
    generic_coords pc = generic_coords::from_indexed(m_coord_getter(p));
    if(c.distance_to(pc) <= r)
      v(p);
  });
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_ball_guarded_order(coord_vector center_,
                                                           coord_scalar r,
                                                           Visitor v)
{
  generic_coords c = generic_coords::from_indexed(center_);
  coord_generator_box<tile_coord_vector> tile_generator(m_tiler(c - r),
                                                        m_tiler(c + r));
  super::visit_region_guarded_order(
      tile_generator, [this, v, &c, &r](particle &p) {
        generic_coords pc = generic_coords::from_indexed(m_coord_getter(p));
        if(c.distance_to(pc) <= r)
          v(p);
      });
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_double_ball(coord_vector c1_,
                                                    coord_vector c2_,
                                                    coord_scalar r,
                                                    Visitor v) const
{
  generic_coords c1 = generic_coords::from_indexed(c1_);
  generic_coords c2 = generic_coords::from_indexed(c2_);

  coord_generator_box<tile_coord_vector> tile_generator(
      m_tiler(std::min(c1, c2) - r), m_tiler(std::max(c1, c2) + r));

  super::visit_region(
      tile_generator, [this, v, &c1, &c2, r](particle const &p) {
        generic_coords pc = generic_coords::from_indexed(m_coord_getter(p));
        if(c1.distance_to(pc) <= r || c2.distance_top(pc))
          v(p);
      });
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_double_ball_dirty(coord_vector c1_,
                                                          coord_vector c2_,
                                                          coord_scalar r,
                                                          Visitor v)
{
  generic_coords c1 = generic_coords::from_indexed(c1_);
  generic_coords c2 = generic_coords::from_indexed(c2_);

  coord_generator_box<tile_coord_vector> tile_generator(
      m_tiler(std::min(c1, c2) - r), m_tiler(std::max(c1, c2) + r));

  super::visit_region_dirty(
      tile_generator, [this, v, &c1, &c2, r](particle &p) {
        generic_coords pc = generic_coords::from_indexed(m_coord_getter(p));
        if(c1.distance_to(pc) <= r || c2.distance_to(pc))
          v(p);
      });
}

template <class Traits>
template <class Visitor>
void cartesian_spacetime<Traits>::visit_double_ball_guarded_order(
    coord_vector c1_, coord_vector c2_, coord_scalar r, Visitor v)
{
  generic_coords c1 = generic_coords::from_indexed(c1_);
  generic_coords c2 = generic_coords::from_indexed(c2_);

  coord_generator_box<tile_coord_vector> tile_generator(
      m_tiler(std::min(c1, c2) - r), m_tiler(std::max(c1, c2) + r));

  super::visit_region_guarded_order(
      tile_generator, [this, v, &c1, &c2, r](particle &p) {
        generic_coords pc = generic_coords::from_indexed(m_coord_getter(p));
        if(c1.distance_to(pc) <= r || c2.distance_to(pc))
          v(p);
      });
}

END_NAMESPACE_CORE

#endif // CARTESIAN_SPACETIME_HPP
