#ifndef ATTRIBUTE_DESCRIPTOR_HPP
#define ATTRIBUTE_DESCRIPTOR_HPP
#include "attribute.hpp"

#include "interface/attribute_kind.hpp"

#include "interface/interface_fwd.hpp"

#include <cstddef>
#include <iosfwd>
#include <string>

BEGIN_NAMESPACE_CORE

class attribute_descriptor
{
public:
  explicit attribute_descriptor(
      std::size_t attr_id = 0, std::string name = "",
      ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INTRINSIC,
      ATTRIBUTE_SCALAR scalar = ATTRIBUTE_SCALAR::UNKNOWN,
      std::size_t attribute_dimension = 0);

  std::string const &attribute_name() const;
  void set_attribute_name(std::string attribute_name);

  std::size_t attribute_idx() const;
  void set_attribute_id(std::size_t attribute_idx);

  ATTRIBUTE_KIND kind() const;
  void set_kind(ATTRIBUTE_KIND kind);

  ATTRIBUTE_SCALAR scalar() const;
  void set_scalar(ATTRIBUTE_SCALAR scalar);

  std::size_t attribute_dimension() const;
  void set_attribute_dimension(std::size_t attribute_dimension);

  static ATTRIBUTE_KIND default_kind(std::string const &name);
  static std::string default_name(ATTRIBUTE_KIND);
  static ATTRIBUTE_SCALAR default_scalar(ATTRIBUTE_KIND);
  static std::size_t default_dimension(ATTRIBUTE_KIND);

  bool set_default_by_name(bool force = false);
  void set_default_by_kind(bool force = false);

private:
  std::size_t m_attribute_id;
  std::string m_attribute_name;
  ATTRIBUTE_KIND m_kind;
  ATTRIBUTE_SCALAR m_scalar;
  std::size_t m_attribue_dimension;
};

std::ostream &operator<<(std::ostream &, attribute_descriptor const &);

END_NAMESPACE_CORE

#endif
