#ifndef SIMBAD_STREAMER_HPP
#define SIMBAD_STREAMER_HPP

#include <core_def.hpp>
#include <core_fwd.hpp>

#include "processors/advancer.hpp"

#include <memory>

BEGIN_NAMESPACE_CORE
class streamer
{
public:
  explicit streamer(model &, property_tree const &);
  explicit streamer(model &, std::unique_ptr<advance_estimator>,
                    std::unique_ptr<stream_printer>);
  ~streamer();

  void launch();

protected:
  std::unique_ptr<advancer> const m_advancer_ptr;
  advance_estimator &m_final_estimator;
  std::unique_ptr<stream_printer> const m_stream_printer_ptr;
};
END_NAMESPACE_CORE
#endif // SIMBAD_STREAMER_HPP
