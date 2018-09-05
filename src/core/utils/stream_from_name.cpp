#include "utils/stream_from_name.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

BEGIN_NAMESPACE_CORE
namespace
{
struct nop
{
  template <class T> void operator()(T const &) const noexcept {}
};
}

ostream_ptr ostream_from_name(std::string const &stream_name)
{
  if(stream_name.empty())
    throw std::runtime_error("recieved empty file name");

  if("STDOUT" == stream_name)
    return ostream_ptr(&std::cout, nop());

  std::ofstream *obj_ptr = new std::ofstream(stream_name);

  return ostream_ptr(obj_ptr, [](std::ostream *ptr) {
    delete dynamic_cast<std::ofstream *>(ptr);
  });
}
ostream_ptr ostream_from_unmanaged(std::ostream *ptr) { return {ptr, nop()}; }

istream_ptr istream_from_name(std::string const &stream_name)
{
  if(stream_name.empty())
    throw std::runtime_error("recieved empty file name");

  if("STDIN" == stream_name)
    return istream_ptr(&std::cin, nop());

  std::ifstream *obj_ptr = new std::ifstream(stream_name);

  return istream_ptr(obj_ptr, [](std::istream *ptr) {
    delete dynamic_cast<std::ifstream *>(ptr);
  });
}
istream_ptr istream_from_unmanaged(std::istream *ptr) { return {ptr, nop()}; }

END_NAMESPACE_CORE