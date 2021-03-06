#include "final_snapshot.hpp"

#include "model_advancer.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/model.hpp"
#include "interface/stream_printer.hpp"
#include "processors/model_advancer.hpp"
#include "repositories/create_from_property_tree.hpp"

#include <assert.h>
#include <boost/optional.hpp>

BEGIN_NAMESPACE_CORE
final_snapshot::final_snapshot(model &model_ref, property_tree const &pt)
    : final_snapshot(model_ref,
                     pt.count("final_estimator")
                         ? factory_create_from_property_tree<advance_estimator>(
                               pt.get_child("final_estimator"))
                         : nullptr,
                     factory_create_from_property_tree<stream_printer>(
                         pt.get_child("configuration_printer")),
                     stacked_view_configuration(
                         model_ref.current_configuration(),
                         pt.get_child("stacked_view", property_tree())))
{
}

final_snapshot::final_snapshot(
    simbad::core::model &model_ref,
    std::unique_ptr<advance_estimator> final_advancer_ptr,
    std::unique_ptr<stream_printer> stream_printer_ptr)
    : final_snapshot(
          model_ref,                     //
          std::move(final_advancer_ptr), //
          std::move(stream_printer_ptr), //
          stacked_view_configuration(model_ref.current_configuration()))
{
}

final_snapshot::final_snapshot(
    simbad::core::model &model_ref,
    std::unique_ptr<advance_estimator> final_advancer_ptr,
    std::unique_ptr<stream_printer> stream_printer_ptr,
    stacked_view_configuration stacked_view)
    : m_advancer_ptr{std::make_unique<model_advancer>(
          model_ref,
          [&]() {
            model_advancer::estimator_ptr_vec vector;
            if(nullptr != final_advancer_ptr)
              vector.push_back(std::move(final_advancer_ptr));
            return vector;
          }())},
      m_final_estimator_ptr(m_advancer_ptr->first_advancer_or_null()),
      m_stream_printer_ptr(std::move(stream_printer_ptr)),
      m_stacked_view(std::move(stacked_view))
{
}

final_snapshot::~final_snapshot() = default;

void final_snapshot::launch()
{
  if(nullptr == m_final_estimator_ptr)
    launch_without_estimator();
  else
    launch_with_estimator();
}

void final_snapshot::launch_without_estimator()
{
  bool can_continue = true;
  do
    can_continue = m_advancer_ptr->advance(default_step);
  while(can_continue);

  m_stream_printer_ptr->write_dataframe(get_model().current_configuration());
}

void final_snapshot::launch_with_estimator()
{
  while(true)
  {
    std::size_t steps_left = m_final_estimator_ptr->estimate();

    if(0 == steps_left)
      break;

    bool ok = m_advancer_ptr->advance(steps_left);
    if(!ok)
      break;
  }
  m_stream_printer_ptr->write_dataframe(get_model().current_configuration());
}

model const &final_snapshot::get_model() const
{
  return m_advancer_ptr->get_model();
}
END_NAMESPACE_CORE