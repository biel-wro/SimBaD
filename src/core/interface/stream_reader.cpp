#include "stream_reader.hpp"

BEGIN_NAMESPACE_CORE

namespace
{
struct nop
{
  template <class T> void operator()(T const &) const noexcept {}
};
}

stream_reader::stream_reader(std::istream *ptr)
    : m_istream_ptr(istream_from_unmanaged(ptr))
{
}

stream_reader::stream_reader(std::istream &ref)
    : m_istream_ptr(istream_from_unmanaged(&ref))
{
}

stream_reader::stream_reader(std::string const &stream_name)
    : m_istream_ptr(istream_from_name(stream_name))
{
}
stream_reader::~stream_reader() = default;
void stream_reader::set_istream(std::istream &ref)
{
  m_istream_ptr = istream_from_unmanaged(&ref);
}
std::istream &stream_reader::istream() const { return *m_istream_ptr; }

END_NAMESPACE_CORE
