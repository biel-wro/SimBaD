#include "model_parameters.hpp"

#include "intrinsic_params.hpp"
#include "particle.hpp"

#include "computational/mutations/builtin_mutators.hpp"
#include "computational/time_dependent_scalars/constant_scalar.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/class_register.hpp"
#include "repositories/create_from_property_tree.hpp"

using simbad::core::get_builtin_mutators;

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

static core::property_tree const &default_time_dependence()
{
  static std::unique_ptr<core::property_tree> const result_ptr{
      new core::property_tree{{"class", "constant_scalar"}}};
  return *result_ptr;
}

static std::unique_ptr<core::time_dependent_scalar>
make_time_dependence(core::property_tree const &pt, std::string const &path)
{
  return core::factory_create_from_property_tree<core::time_dependent_scalar>(
      pt.get_child(path, default_time_dependence()));
}

model_params::model_params(const simbad::core::property_tree &pt)
    : m_mutation_prob(pt.get<double>("mutation.probability")),
      m_interaction(pt.get_child("interaction")),
      m_dispersion(pt.get_child("birth.dispersion")),
      m_birth_extractor(pt.get_child("birth.saturation")),
      m_lifespan_extractor(pt.get_child("lifespan.saturation")),
      m_success_extractor(pt.get_child("success.saturation")),
      m_birth_eff_mutator_ptr(get_builtin_mutators().create_instance(
          pt.get_child("birth.efficiency.mutator"))),
      m_birth_res_mutator_ptr(get_builtin_mutators().create_instance(
          pt.get_child("birth.resistance.mutator"))),
      m_lifespan_eff_mutator_ptr(get_builtin_mutators().create_instance(
          pt.get_child("lifespan.efficiency.mutator"))),
      m_lifespan_res_mutator_ptr(get_builtin_mutators().create_instance(
          pt.get_child("lifespan.resistance.mutator"))),
      m_success_eff_mutator_ptr(get_builtin_mutators().create_instance(
          pt.get_child("success.efficiency.mutator"))),
      m_success_res_mutator_ptr(get_builtin_mutators().create_instance(
          pt.get_child("success.resistance.mutator"))),
      m_success_eff_time_dep(
          make_time_dependence(pt, "success.efficiency.time_dependency")),
      m_success_res_time_dep(
          make_time_dependence(pt, "succeess.resistane.time_dependency"))

{
}
model_params::~model_params() {}
double model_params::birth_rate(double density, double eff, double res) const
{
  double val = eff * birth_saturation(density / res);
  return val;
}

double model_params::lifespan(double density, double eff, double res) const
{
  double val = eff * lifespan_saturation(density / res);
  return val;
}

double model_params::success_prob(double density, double eff, double res) const
{
  double val = eff * success_saturation(density / res);
  return val;
}

bool model_params::sample_mutation(cell &cp, std::mt19937_64 &rng) const
{
  return m_mutation_prob >= std::uniform_real_distribution<>()(rng);
}

const model_params::interaction_type &model_params::interaction() const
{
  return m_interaction;
}

template <class RealType, class Mutator, class RNG>
static void apply_mutator(RealType &val, Mutator const &mutator, RNG &rng)
{
  using StoreRealType = typename Mutator::real_type;
  RealType res = mutator(StoreRealType(val), rng);
  val = StoreRealType(res);
}

double model_params::birth_saturation(double x) const
{
  return m_birth_extractor(x);
}
double model_params::lifespan_saturation(double x) const
{
  return m_lifespan_extractor(x);
}
double model_params::success_saturation(double x) const
{
  return m_success_extractor(x);
}

void model_params::mutate_birth(cell_params &cp, std::mt19937_64 &rnd) const
{
  apply_mutator(cp.birth_eff(), *m_birth_eff_mutator_ptr, rnd);
  apply_mutator(cp.birth_res(), *m_birth_res_mutator_ptr, rnd);
}
void model_params::mutate_lifespan(cell_params &cp, std::mt19937_64 &rnd) const
{
  apply_mutator(cp.lifespan_eff(), *m_lifespan_eff_mutator_ptr, rnd);
  apply_mutator(cp.lifespan_res(), *m_lifespan_eff_mutator_ptr, rnd);
}

void model_params::mutate_success(cell_params &cp, std::mt19937_64 &rnd) const
{
  apply_mutator(cp.success_eff(), *m_success_eff_mutator_ptr, rnd);
  apply_mutator(cp.success_res(), *m_success_res_mutator_ptr, rnd);
}

const simbad::models::parameter_evolution_3d::model_params::dispersion_type &
simbad::models::parameter_evolution_3d::model_params::dispersion() const
{
  return m_dispersion;
}

END_NAMESPACE_PARAMETER_EVOLUTION_3D
