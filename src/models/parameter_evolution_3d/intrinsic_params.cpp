#include "intrinsic_params.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"

#include <limits>
BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

static std::unique_ptr<core::attribute_description> make_description()
{
  std::unique_ptr<core::attribute_description> description_ptr(
      new core::attribute_description);
  core::attribute_description &desc = *description_ptr;
  core::ATTRIBUTE_KIND INTRINSIC = core::ATTRIBUTE_KIND::INTRINSIC;
  core::ATTRIBUTE_SCALAR REAL = core::ATTRIBUTE_SCALAR::REAL;

  desc.add_attribute_auto_idx("birth.efficiency", INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("birth.resistance", INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("lifespan.efficiency", INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("lifespan.resistance", INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("success.efficiency", INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("succes.resistance", INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("mutation.id", INTRINSIC,
                              core::ATTRIBUTE_SCALAR::INT, 1);

  return description_ptr;
}

const core::attribute_description &cell_params::description()
{
  static std::unique_ptr<core::attribute_description> desc_ptr(
      make_description());
  return *desc_ptr;
}

cell_params::cell_params(const simbad::core::property_tree &pt)
    : cell_params(pt.get<float>("birth.efficiency"),     //
                  pt.get<float>("birth.resistance"),     //
                  pt.get<float>("lifespan.efficiency"),  //
                  pt.get<float>("lifespan.resistance"),  //
                  pt.get<float>("success.efficiency"),   //
                  pt.get<float>("success.resistance"),   //
                  pt.get("mutation.id", std::size_t(0)), //
                  nullptr)                               //
{
}

cell_params::cell_params(const core::attribute_list &p,
                         std::vector<std::size_t> const &attribute_indices)
    : cell_params(p[attribute_indices[0]].get_real_val(),
                  p[attribute_indices[1]].get_real_val(),
                  p[attribute_indices[2]].get_real_val(),
                  p[attribute_indices[3]].get_real_val(),
                  p[attribute_indices[4]].get_real_val(),
                  p[attribute_indices[5]].get_real_val(),
                  p[attribute_indices[6]].get_int_val(), nullptr)
{
}
cell_params::cell_params(float birth_eff, float birth_res, float lifespan_eff,
                         float lifespan_res, float success_eff,
                         float success_res, std::size_t mutation_id,
                         std::shared_ptr<const cell_params> parent_ptr)
    : m_parent_ptr(parent_ptr),
      m_mutation_id(mutation_id),
      m_birth_eff(birth_eff),
      m_birth_res(birth_res),
      m_lifespan_eff(lifespan_eff),
      m_lifespan_res(lifespan_res),
      m_success_eff(success_eff),
      m_success_res(success_res)
{
}

core::attribute cell_params::get_attribute(std::size_t attr_idx) const
{
  switch(attr_idx)
  {
  case 0: return birth_eff();
  case 1: return birth_res();
  case 2: return lifespan_eff();
  case 3: return lifespan_res();
  case 4: return success_eff();
  case 5: return success_res();
  case 6: return mutation_id();
  default: throw core::unrecognized_attribute_number(attr_idx);
  }
}

std::vector<std::size_t> cell_params::get_attribute_indices(
    simbad::core::attribute_description const &mapping)
{
  return {
      mapping["birth.efficiency"].attribute_idx(),
      mapping["birth.resistance"].attribute_idx(),
      mapping["lifespan.efficiency"].attribute_idx(),
      mapping["lifespan.resistance"].attribute_idx(),
      mapping["success.efficiency"].attribute_idx(),
      mapping["success.resistance"].attribute_idx(),
      mapping["mutation.id"].attribute_idx(),
      // mapping["mutation.parent.id"].attribute_idx(),
  };
}

std::size_t cell_params::mutation_id() const { return m_mutation_id; }
void cell_params::set_mutation_id(std::size_t mutation_id)
{
  m_mutation_id = mutation_id;
}

std::shared_ptr<cell_params const> cell_params::parent_ptr() const
{
  return m_parent_ptr;
}

void cell_params::set_parent_ptr(std::shared_ptr<const cell_params> parent_ptr)
{
  m_parent_ptr = parent_ptr;
}

core::attribute cell_params::ancestry_ids() const
{
  cell_params const *parent_ptr = this->parent_ptr().get();
  if(!parent_ptr)
    return core::attribute{mutation_id()};

  cell_params const *parent2_ptr = parent_ptr->parent_ptr().get();
  if(!parent2_ptr)
    return core::attribute{mutation_id(), parent_ptr->mutation_id()};

  cell_params const *parent3_ptr = parent2_ptr->parent_ptr().get();
  if(!parent3_ptr)
    return core::attribute{mutation_id(), parent_ptr->mutation_id(),
                           parent2_ptr->mutation_id()};

  using int_type = core::attribute::int_type;
  core::attribute::intn_type vector{
      static_cast<int_type>(mutation_id()),
      static_cast<int_type>(parent_ptr->mutation_id()),
      static_cast<int_type>(parent2_ptr->mutation_id()),
      static_cast<int_type>(parent3_ptr->mutation_id())};

  for(cell_params const *parentn_ptr = parent3_ptr->parent_ptr().get();
      nullptr != parentn_ptr; parentn_ptr = parentn_ptr->parent_ptr().get())
    vector.push_back(static_cast<int_type>(parentn_ptr->mutation_id()));
  return vector;
}

float cell_params::lifespan_eff() const { return m_lifespan_eff; }
float &cell_params::lifespan_eff() { return m_lifespan_eff; }
float cell_params::lifespan_res() const { return m_lifespan_res; }
float &cell_params::lifespan_res() { return m_lifespan_res; }
float cell_params::birth_eff() const { return m_birth_eff; }
float &cell_params::birth_eff() { return m_birth_eff; }
float cell_params::birth_res() const { return m_birth_res; }
float &cell_params::birth_res() { return m_birth_res; }
float cell_params::success_eff() const { return m_success_eff; }
float &cell_params::success_eff() { return m_success_eff; }
float cell_params::success_res() const { return m_success_res; }
float &cell_params::success_res() { return m_success_res; }
END_NAMESPACE_PARAMETER_EVOLUTION_3D
