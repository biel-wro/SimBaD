#ifndef GENERIC_PARTICLE_HPP
#define GENERIC_PARTICLE_HPP
#include "core_fwd.hpp"

#include "interface/attribute.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"

#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

BEGIN_NAMESPACE_CORE
class generic_particle
{
public:
  constexpr static std::size_t NOT_STORED =
      std::numeric_limits<std::size_t>::max();
  // Constructors
  generic_particle();

  void visit_view(configuration_view::particle_visitor v
                  //,std::size_t id_attr_idx = NOT_STORED,
                  //std::size_t coord_attr_idx = NOT_STORED
                  ) const;

  void clear_attributes();
  void set_attribute(std::size_t extra_idx, attribute attr);
  attribute const &find_attribute(std::size_t idx) const;

  std::vector<attribute> &attributes();
  std::vector<attribute> const &attributes() const;

  bool operator==(generic_particle const &rhs) const;
  bool operator!=(generic_particle const &rhs) const;

private:
  std::vector<attribute> m_attributes;
};
END_NAMESPACE_CORE

namespace std
{
template <> struct hash<simbad::core::generic_particle>
{
  std::size_t operator()(simbad::core::generic_particle const &p) const;
};
}
#endif // GENERIC_PARTICLE_HPP
