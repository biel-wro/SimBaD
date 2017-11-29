#ifndef SIMBAD_CORE_STREAM_READER_HPP
#define SIMBAD_CORE_STREAM_READER_HPP

#include "core_fwd.hpp"
#include <functional>
#include <iosfwd>

BEGIN_NAMESPACE_CORE
class stream_reader
{
public:
  using entry_visitor = std::function<void(attribute_list const &)>;

  stream_reader(std::istream *istream);
  stream_reader(std::istream &istream);
  void set_istream(std::istream &istream);
  std::istream &istream() const;

  virtual attribute_description read_header() = 0;

  virtual void visit_entries(entry_visitor v, std::size_t max_reads = 0) = 0;

private:
  std::istream *m_istream_ptr;
};
END_NAMESPACE_CORE

#endif // SIMBAD_STREAM_READER_HPP
