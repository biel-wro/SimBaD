#ifndef ATTRIBUTE_DESCRIPTOR_HPP
#define ATTRIBUTE_DESCRIPTOR_HPP
#include "attribute.hpp"
#include "interface/interface_fwd.hpp"

#include <cstddef>
#include <string>

BEGIN_NAMESPACE_CORE
enum class ATTRIBUTE_KIND : std::uint32_t
{
  /* Special attributes */
  PARTICLE_UID, // optional, unique, integer, scalar
  EVENT_UID,    // optional, unique, integer, scalar
  POSITION,     // optional, numeric values, fixed dimension
  TIME,         // numeric scalar
  DELTA_TIME,   // integer scalar
  EVENT_KIND,   // special case, same values as in event_kind enum
  /* Model-dependent attributes */
  INTRINSIC,   // attributes stored in particle, non-recomputable
  ACCUMULATED, // attributes stored in particle, recomputable
  OBSERVABLE,  // attributes not-stored in particle, with physical meaning
  INFO         // other attributes
};

class attribute_descriptor
{
public:
  /*explicit attribute_descriptor(std::size_t attr_id, std::string name,
                                ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INTRINSIC,
                                std::size_t attribute_dimension = 0);*/
  explicit attribute_descriptor(
      std::size_t attr_id, std::string name,
      ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INTRINSIC,
      ATTRIBUTE_SCALAR scalar = ATTRIBUTE_SCALAR::UNKNOWN,
      std::size_t attribute_dimension = 0);

  std::string const &attribute_name() const;
  void set_attribute_name(std::string const &attribute_name);

  std::size_t attribute_idx() const;
  void set_attribute_id(std::size_t attribute_idx);

  ATTRIBUTE_KIND kind() const;
  void set_kind(ATTRIBUTE_KIND kind);

  ATTRIBUTE_SCALAR scalar() const;
  void set_scalar(ATTRIBUTE_SCALAR scalar);

  std::size_t attribute_dimension() const;
  void set_attribute_dimension(std::size_t attribute_dimension);

private:
  std::size_t m_attribute_id;
  std::string m_attribute_name;
  ATTRIBUTE_KIND m_kind;
  ATTRIBUTE_SCALAR m_scalar;
  std::size_t m_attribue_dimension;
};

END_NAMESPACE_CORE
#endif
