#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP
#include "parameter_evolution_3d_def.hpp"

#include "property_tree.hpp"
#include "extractors/generalized_exponential_extractor.hpp"


BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct model_parameters
{ 
  model_parameters();
  ~model_parameters();

  double interaction_range;
  double birth_dispersion;
  simbad::core::generalized_exponential_extractor<double> m_extractor;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARAMETERS_HPP
