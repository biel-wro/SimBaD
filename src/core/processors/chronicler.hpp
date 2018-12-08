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

  chronicler(stream_reader &,                                               //
             std::unique_ptr<stream_printer> stream_printer_ptr,
             std::string const &key_name,
             std::vector<std::string> const &nonkey_names);
  chronicler(stream_reader &, property_tree const &pt);

  ~chronicler();

  void launch();

  void set_initial_configuration(configuration_view const &);
  configuration_reader &reader();
  printing_chronicle_builder &builder();
protected:
private:
  stream_reader &m_reader;

  std::unique_ptr<stream_printer> const m_stream_printer_ptr;
  std::unique_ptr<printing_chronicle_builder> m_builder_ptr;
};

END_NAMESPACE_CORE
#endif // SIMBAD_PROCESSOR_CHRONICLES_HPP
