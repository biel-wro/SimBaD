#include "snapshoter.hpp"

#include "advancer.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/model.hpp"
#include "interface/stream_printer.hpp"
#include "processors/advancer.hpp"
#include "repositories/create_from_property_tree.hpp"

#include <assert.h>
#include <boost/optional.hpp>
#include <iterator>

BEGIN_NAMESPACE_CORE

snapshoter::snapshoter(model &model_ref, property_tree const &pt)
    : snapshoter(model_ref,
                 factory_create_from_property_tree<advance_estimator>(
                     pt.get_child("step_estimator")),
                 factory_create_from_property_tree<advance_estimator>(
                     pt.get_child("final_estimator")),
                 factory_create_from_property_tree<stream_printer>(
                     pt.get_child("configuration_printer")),
                 stacked_view_configuration(model_ref.current_configuration(),
                                            pt.get_child("stacked_view")))
{
}

snapshoter::snapshoter(simbad::core::model &model_ref,
                       std::unique_ptr<advance_estimator> step_advancer_ptr,
                       std::unique_ptr<advance_estimator> final_advancer_ptr,
                       std::unique_ptr<stream_printer> stream_printer_ptr)
    : snapshoter(model_ref, //
                 std::move(step_advancer_ptr), std::move(final_advancer_ptr),
                 std::move(stream_printer_ptr),
                 stacked_view_configuration(model_ref.current_configuration()))
{
}

snapshoter::snapshoter(model &model_ref,
                       std::unique_ptr<advance_estimator> step_advancer_ptr,
                       std::unique_ptr<advance_estimator> final_advancer_ptr,
                       std::unique_ptr<stream_printer> stream_printer_ptr,
                       stacked_view_configuration stacked_view)
    : m_advancer_ptr{std::make_unique<advancer>(
          model_ref,
          [&]() {
            advancer::estimator_ptr_vec vector;
            vector.push_back(std::move(step_advancer_ptr));
            vector.push_back(std::move(final_advancer_ptr));
            return vector;
          }())},
      m_step_estimator(*m_advancer_ptr->begin()),
      m_final_estimator(*std::next(m_advancer_ptr->begin())),
      m_stream_printer_ptr(std::move(stream_printer_ptr)),
      m_stacked_view(std::move(stacked_view))
{
}

snapshoter::~snapshoter() = default;
void snapshoter::launch()
{
  bool should_continue;
  do
  {
    m_stacked_view.set_base(get_model().current_configuration());

    m_stream_printer_ptr->write_dataframe(m_stacked_view);

    should_continue = next_step();

  } while(should_continue);
}

model const &snapshoter::get_model() const
{
  return m_advancer_ptr->get_model();
}

bool snapshoter::next_step()
{
  std::size_t step_estimate;
  do
  {
    std::size_t final_estimate = m_final_estimator.estimate();
    if(0 == final_estimate)
      return false;

    bool next_step_exist = m_step_estimator.next_target();
    if(!next_step_exist)
      return false;

    step_estimate = m_step_estimator.estimate();

    std::size_t next_step = std::min(final_estimate, step_estimate);

    m_advancer_ptr->advance(next_step);
  } while(0 == step_estimate);

  return true;
}

END_NAMESPACE_CORE
