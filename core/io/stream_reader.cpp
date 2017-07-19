#include "stream_reader.hpp"

simbad::core::stream_reader::stream_reader(std::istream *istream)
    : m_istream_ptr(istream)
{
}

simbad::core::stream_reader::stream_reader(std::istream &istream)
    : m_istream_ptr(&istream)
{
}

void simbad::core::stream_reader::set_istream(std::istream &istream)
{
  m_istream_ptr = &istream;
}
std::istream &simbad::core::stream_reader::istream() const {
  return *m_istream_ptr;
}


BEGIN_NAMESPACE_CORE

END_NAMESPACE_CORE
