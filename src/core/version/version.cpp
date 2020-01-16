#include "version/version.hpp"
#include <iostream>

BEGIN_NAMESPACE_CORE
Version::Version(unsigned major, unsigned minor, unsigned patch)
    : major_version(major), minor_version(minor), patch_version(patch)
{
}

std::ostream &operator<<(std::ostream &o, Version const &v)
{
  o << v.major_version << '.' << v.minor_version << '.' << v.patch_version;
  return o;
}

Version const &SimBaD_core_version()
{
  static Version const v(SIMBAD_VERSION_MAJOR, // values received from CMake
                         SIMBAD_VERSION_MINOR, //
                         SIMBAD_VERSION_PATCH);
  return v;
}
END_NAMESPACE_CORE
