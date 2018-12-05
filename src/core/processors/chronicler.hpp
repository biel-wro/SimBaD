#ifndef SIMBAD_PROCESSORS_CHRONICLES_HPP
#define SIMBAD_PROCESSORS_CHRONICLES_HPP

#include "interface/interface_fwd.hpp"
#include "processors_fwd.hpp"

#include <memory>
#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE

class chronicler
{
public:
  static constexpr std::size_t default_advancement_step = 10000;

  chronicler(event_source &,                                               //
             std::unique_ptr<stream_printer> stream_printer_ptr,
             std::string const &key_name,
             std::vector<std::string> const &nonkey_names);
  chronicler(event_source &, property_tree const &pt);

  ~chronicler();

  void launch();

protected:
private:
  event_source &m_event_source;

  std::unique_ptr<stream_printer> const m_stream_printer_ptr;
  std::unique_ptr<printing_chronicle_builder> m_builder_ptr;
};

END_NAMESPACE_CORE
#endif // SIMBAD_PROCESSOR_CHRONICLES_HPP
