#ifndef INTRINSIC_PARAMS_HPP
#define INTRINSIC_PARAMS_HPP

#include "parameter_evolution_3d_fwd.hpp"

#include "core_fwd.hpp"
#include "interface/attribute_description.hpp"

#include <random>
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

struct cell_params
{
public:
  // static members
  static core::attribute_description const &description();
  static std::vector<std::size_t>
  get_attribute_indices(simbad::core::attribute_description const &mapping);

  // constructors
  cell_params(simbad::core::property_tree const &pt);
  cell_params(simbad::core::attribute_list const &p,
              std::vector<std::size_t> const &attribute_indices);
  cell_params(float birth_eff, float birth_res, float lifespan_eff,
              float lifespan_res, float success_eff, float success_res,
              std::size_t mutation_id,
              std::shared_ptr<cell_params const> parent_ptr = nullptr);

  // accessors
  core::attribute get_attribute(std::size_t attr_idx) const;

  std::size_t mutation_id() const;
  void set_mutation_id(std::size_t mutation_id);

  std::shared_ptr<cell_params const> parent_ptr() const;
  void set_parent_ptr(std::shared_ptr<cell_params const> parent_ptr);
  core::attribute ancestry_ids() const;
  float lifespan_eff() const;
  float &lifespan_eff();
  float lifespan_res() const;
  float &lifespan_res();
  float birth_eff() const;
  float &birth_eff();
  float birth_res() const;
  float &birth_res();
  float success_eff() const;
  float &success_eff();
  float success_res() const;
  float &success_res();

private:
  std::shared_ptr<cell_params const> m_parent_ptr;
  std::size_t m_mutation_id;
  float m_birth_eff;
  float m_birth_res;
  float m_lifespan_eff;
  float m_lifespan_res;
  float m_success_eff;
  float m_success_res;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // INTRINSIC_PARAMS_HPP
