#ifndef SIMBAD_STREAM_FROM_NAME_HPP
#define SIMBAD_STREAM_FROM_NAME_HPP

#include "core_def.hpp"

#include <functional>
#include <iosfwd>
#include <string>

BEGIN_NAMESPACE_CORE
using ostream_deleter = std::function<void(std::ostream *)>;
using ostream_ptr = std::unique_ptr<std::ostream, ostream_deleter>;
ostream_ptr ostream_from_name(std::string const &name);
ostream_ptr ostream_from_unmanaged(std::ostream *);

using istream_deleter = std::function<void(std::istream *)>;
using istream_ptr = std::unique_ptr<std::istream, istream_deleter>;
istream_ptr istream_from_name(std::string const &name);
istream_ptr istream_from_unmanaged(std::istream *);

END_NAMESPACE_CORE

#endif // SIMBAD_STREAM_FROM_NAME_HPP
