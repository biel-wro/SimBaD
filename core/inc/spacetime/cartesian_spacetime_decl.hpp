#ifndef CARTESIAN_SPACETIME_DECL_HPP
#define CARTESIAN_SPACETIME_DECL_HPP
#include "core_def.hpp"

#include "coordinates/coordinates.hpp"
#include "coordinates/scale_coord_tiler.hpp"
#include "spacetime/spacetime_decl.hpp"

BEGIN_NAMESPACE_CORE

template <class ParticleTraits> struct make_spacetime_traits_cartesian
{
  struct type
  {
    using particle_type = typename ParticleTraits::particle;
    using particle_ordering = typename ParticleTraits::ordering;
    using coord_getter = typename ParticleTraits::coord_getter;
    using coord_scalar = typename ParticleTraits::coord_scalar;
    using tile_coord_scalar = int;

    using dimension_type = typename ParticleTraits::dimension_type;
    constexpr static dimension_type dimension = ParticleTraits::dimension;

    using tile_key = coordinates<tile_coord_scalar, ParticleTraits::dimension>;
    using tile_key_equal = std::equal_to<tile_key>;
    using tile_key_hash = coord_hasher;
  };
};

template <class ParticleTraits>
class cartesian_spacetime
    : public spacetime<
          typename make_spacetime_traits_cartesian<ParticleTraits>::type>
{
public:
  using spacetime_traits =
      typename make_spacetime_traits_cartesian<ParticleTraits>::type;

  using super = spacetime<spacetime_traits>;

  using particle_traits = ParticleTraits;
  using particle = typename particle_traits::particle;
  using ordering = typename particle_traits::ordering;
  using coord_scalar = typename particle_traits::coord_scalar;
  using coord_vector = typename particle_traits::coord_vector;
  using coord_getter = typename particle_traits::coord_getter;
  using dimension_type = typename particle_traits::dimension_type;
  constexpr static dimension_type dimension = particle_traits::dimension;

  using generic_coords = coordinates<coord_scalar, dimension>;

  using tile_coord_scalar = int;
  using tile_coord_vector = coordinates<tile_coord_scalar, dimension>;

  using tiler = scale_coord_tiler<coord_scalar, tile_coord_scalar, dimension>;

  using dirty_handle_type = typename super::dirty_handle_type;
  using const_handle_type = typename super::const_handle_type;

  // Constructors
  explicit cartesian_spacetime(coord_scalar tile_size = 1,
                               ordering ord = ordering(),
                               coord_getter cg = coord_getter());
  template <class... Args>
  const_handle_type emplace(coord_vector const &cv, Args... args);
  template <class... Args>
  dirty_handle_type emplace_dirty(coord_vector const &cv, Args... args);

  template <class P> const_handle_type insert(P &&p);
  template <class P> dirty_handle_type insert_dirty(P &&p);

  template <class Visitor>
  void visit_ball(coord_vector center, coord_scalar radius,
                  Visitor v = Visitor()) const;
  template <class Visitor>
  void visit_ball_dirty(coord_vector center, coord_scalar radius,
                  Visitor v = Visitor());
  template <class Visitor>
  void visit_ball_guarded_order(coord_vector center, coord_scalar radius,
                  Visitor v = Visitor());

  tiler m_tiler;
  coord_getter m_coord_getter;
};

END_NAMESPACE_CORE
#endif // CARTESIAN_SPACETIME_DECL_HPP
