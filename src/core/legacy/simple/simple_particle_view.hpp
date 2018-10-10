#ifndef SIMPLE_PARTICLE_VIEW_HPP
#define SIMPLE_PARTICLE_VIEW_HPP

#include "interface/interface_fwd.hpp"
#include "interface/particle.hpp"
BEGIN_NAMESPACE_CORE

template <class Particle> class simple_particle_view : public particle
{
public:
  simple_particle_view() : p_base(nullptr) {}
  simple_particle_view(Particle const &p) : p_base(&p) {}
  // std::size_t dimension() const override { return p_base->dimension(); }
  double coord(std::size_t d) const override { return p_base->coordinate(d); }
protected:
  Particle const *p_base;
};

END_NAMESPACE_CORE
#endif // SIMPLE_PARTICLE_VIEW_HPP
