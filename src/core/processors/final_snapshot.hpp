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
  static constexpr std::size_t default_step = 10000;
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
  void launch_without_estimator();
  void launch_with_estimator();

  model const &get_model() const;

private:
  std::unique_ptr<advancer> const m_advancer_ptr;
  advance_estimator *const m_final_estimator_ptr; // can be nullptr
  std::unique_ptr<stream_printer> const m_stream_printer_ptr;
  stacked_view_configuration m_stacked_view;
};
END_NAMESPACE_CORE

#endif // SIMBAD_FINAL_SNAPSHOT_HPP
