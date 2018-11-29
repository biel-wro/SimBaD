#ifndef SIMBAD_PROCESSORS_CHRONICLES_HPP
#define SIMBAD_PROCESSORS_CHRONICLES_HPP

#include "core_fwd.hpp"
#include "interface/interface_fwd.hpp"
#include "processors_fwd.hpp"

#include "memory"
BEGIN_NAMESPACE_CORE

class chronicler
{
public:
  constexpr std::size_t default_advancement_step =  10000;

  chronicler(model &, property_tree const &pt);
  chronicler(model &,                                               //
             std::unique_ptr<advance_estimator> final_advancer_ptr, //
             std::unique_ptr<stream_printer> stream_printer_ptr);
  ~chronicler();

  void launch();

protected:
  void launch_without_estimator();
  void launch_with_estimator();

private:
  std::unique_ptr<advancer> const m_advancer_ptr;
  advance_estimator *const m_final_estimator; // can be null
  std::unique_ptr<stream_printer> const m_stream_printer_ptr;

  struct builder;
  std::unique_ptr<builder> m_builder_ptr;
};

END_NAMESPACE_CORE
#endif // SIMBAD_PROCESSOR_CHRONICLES_HPP
