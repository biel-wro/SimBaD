#ifndef SIMBAD_VERSION_HPP
#define SIMBAD_VERSION_HPP

#include <iosfwd>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE
struct Version
{
  explicit Version(unsigned major, unsigned minor, unsigned patch);
  unsigned major_version;
  unsigned minor_version;
  unsigned patch_version;
};

Version const &SimBaD_core_version();

std::ostream &operator<<(std::ostream &, Version const&);
END_NAMESPACE_CORE

#endif // SIMBAD_VERSION_HPP
