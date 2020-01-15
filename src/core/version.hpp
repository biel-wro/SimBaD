#ifndef SIMBAD_VERSION_HPP
#define SIMBAD_VERSION_HPP

#define SIMBAD_VERSION_MAJOR 1
#define SIMBAD_VERSION_MINOR 0
#define SIMBAD_VERSION_PATCH 0

#define SIMBAD_MAKE_STRING(STR) #STR
#define SIMBAD_VERSION_MAKE_STRING(MAJOR, MINOR, PATCH)                        \
  SIMBAD_MAKE_STRING(MAJOR)                                                    \
  "." SIMBAD_MAKE_STRING(MINOR) "." SIMBAD_MAKE_STRING(PATCH)

#define SIMBAD_VERSION_STRING                                                  \
  SIMBAD_VERSION_MAKE_STRING(SIMBAD_VERSION_MAJOR, SIMBAD_VERSION_MINOR,       \
                             SIMBAD_VERSION_PATCH)

#endif // SIMBAD_VERSION_HPP
