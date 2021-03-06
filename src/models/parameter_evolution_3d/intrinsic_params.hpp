#ifndef INTRINSIC_PARAMS_HPP
#define INTRINSIC_PARAMS_HPP

#include "parameter_evolution_3d_fwd.hpp"

#include "interface/attribute_description.hpp"
#include "interface/interface_fwd.hpp"

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
  cell_params(float birth_eff, float birth_res, float death_eff,
              float death_res, float success_eff, float success_res,
              std::size_t mutation_id
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
              ,
              std::shared_ptr<cell_params const> parent_ptr = nullptr
#endif
  );

  // accessors
  core::attribute get_attribute(std::size_t attr_idx) const;

  std::size_t mutation_id() const;
  void set_mutation_id(std::size_t mutation_id);
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
  std::shared_ptr<cell_params const> parent_ptr() const;
  void set_parent_ptr(std::shared_ptr<cell_params const> parent_ptr);
  core::attribute ancestry_ids() const;
#endif
  float death_eff() const;
  float &death_eff();
  float death_res() const;
  float &death_res();
  float birth_eff() const;
  float &birth_eff();
  float birth_res() const;
  float &birth_res();
  float success_eff() const;
  float &success_eff();
  float success_res() const;
  float &success_res();

private:
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
  std::shared_ptr<cell_params const> m_parent_ptr;
#endif
  std::size_t m_mutation_id;
  float m_birth_eff;
  float m_birth_res;
  float m_death_eff;
  float m_death_res;
  float m_success_eff;
  float m_success_res;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // INTRINSIC_PARAMS_HPP
