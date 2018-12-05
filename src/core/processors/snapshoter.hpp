#ifndef SNAPSHOTTER_HPP
#define SNAPSHOTTER_HPP

#include "interface/interface_fwd.hpp"
#include <configurations/stacked_view_configuration.hpp>

#include <memory>

BEGIN_NAMESPACE_CORE
class snapshoter
{
public:
  using time_type = double;

  snapshoter(model &, property_tree const &pt);

  snapshoter(model &, //
             std::unique_ptr<advance_estimator> step_advancer_ptr,
             std::unique_ptr<advance_estimator> final_advancer_ptr,
             std::unique_ptr<stream_printer> stream_printer_ptr);

  snapshoter(model &, //
             std::unique_ptr<advance_estimator> step_advancer_ptr,
             std::unique_ptr<advance_estimator> final_advancer_ptr,
             std::unique_ptr<stream_printer> stream_printer_ptr,
             stacked_view_configuration);

  ~snapshoter();

  void launch();

protected:
  model const &get_model() const;
  bool next_step();

private:
  std::unique_ptr<model_advancer> const m_advancer_ptr;
  advance_estimator &m_step_estimator;
  advance_estimator &m_final_estimator;
  std::unique_ptr<stream_printer> const m_stream_printer_ptr;
  stacked_view_configuration m_stacked_view;
};
END_NAMESPACE_CORE
#endif // SNAPSHOTTER_HPP
