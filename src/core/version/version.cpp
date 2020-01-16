#include "version/version.hpp"
#include <iostream>

//#define SIMBAD_VERSION_MAJOR 1
//#define SIMBAD_VERSION_MINOR 0
//#define SIMBAD_VERSION_PATCH 0
/*#define SIMBAD_MAKE_STRING(STR) #STR
#define SIMBAD_VERSION_MAKE_STRING(MAJOR, MINOR, PATCH)                        \
  SIMBAD_MAKE_STRING(MAJOR)                                                    \
  "." SIMBAD_MAKE_STRING(MINOR) "." SIMBAD_MAKE_STRING(PATCH)

#define SIMBAD_VERSION_STRING                                                  \
  SIMBAD_VERSION_MAKE_STRING(SIMBAD_VERSION_MAJOR, SIMBAD_VERSION_MINOR,       \
                             SIMBAD_VERSION_PATCH)
*/
BEGIN_NAMESPACE_CORE
Version::Version(unsigned major, unsigned minor, unsigned patch)
    : major(major), minor(minor), patch(patch)
{
}

std::ostream &operator<<(std::ostream &o, Version const &v)
{
  o << v.major << '.' << v.minor << '.' << v.patch;
  return o;
}

Version const &SimBaD_core_version()
{
  static Version const v(SIMBAD_VERSION_MAJOR, SIMBAD_VERSION_MINOR,
                   SIMBAD_VERSION_PATCH);
  return v;
}
END_NAMESPACE_CORE
