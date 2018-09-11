#ifndef SIMBAD_FINAL_SNAPSHOT_HPP
#define SIMBAD_FINAL_SNAPSHOT_HPP

#include "interface/interface_fwd.hpp"
#include <configurations/stacked_view_configuration.hpp>

#include <memory>

BEGIN_NAMESPACE_CORE
class final_snapshot
{
public:
  using time_type = double;

  final_snapshot(model &, property_tree const &pt);

  final_snapshot(model &, //
             std::unique_ptr<advance_estimator> final_advancer_ptr,
             std::unique_ptr<stream_printer> stream_printer_ptr);

  final_snapshot(model &, //
             std::unique_ptr<advance_estimator> final_advancer_ptr,
             std::unique_ptr<stream_printer> stream_printer_ptr,
             stacked_view_configuration);

  ~final_snapshot();

  void launch();

protected:
  model const &get_model() const;

private:
  std::unique_ptr<advancer> const m_advancer_ptr;
  advance_estimator &m_final_estimator;
  std::unique_ptr<stream_printer> const m_stream_printer_ptr;
  stacked_view_configuration m_stacked_view;
};
END_NAMESPACE_CORE

#endif //SIMBAD_FINAL_SNAPSHOT_HPP
