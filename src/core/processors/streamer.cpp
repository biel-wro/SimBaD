#include "streamer.hpp"

#include "interface/stream_printer.hpp"
#include "processors/model_advancer.hpp"
#include "repositories/create_from_property_tree.hpp"

BEGIN_NAMESPACE_CORE

streamer::streamer(model &m, property_tree const &pt)
    : streamer{m, factory_create_from_property_tree<advance_estimator>(
                      pt.get_child("final_estimator")),
               factory_create_from_property_tree<stream_printer>(
                   pt.get_child("stream_printer"))}
{
}

streamer::streamer(model &m,
                   std::unique_ptr<advance_estimator> final_estimator_ptr,
                   std::unique_ptr<stream_printer> stream_printer_ptr)
    : m_advancer_ptr{new model_advancer{m,
                                  [&]() {
                                    model_advancer::estimator_ptr_vec vector;
                                    vector.push_back(
                                        std::move(final_estimator_ptr));
                                    return vector;
                                  }()}},
      m_final_estimator(*m_advancer_ptr->begin()),
      m_stream_printer_ptr{std::move(stream_printer_ptr)}
{
}
streamer::~streamer() = default;

void streamer::launch()
{
  m_stream_printer_ptr->write_header(
      m_advancer_ptr->get_model().event_description());

  model::event_visitor event_visitor = [&printer(*m_stream_printer_ptr)](
      attribute_list const &event)
  {
    printer.write_entry(event);
  };

  for(std::size_t step_estimate = m_final_estimator.estimate();
      0 != step_estimate; step_estimate = m_final_estimator.estimate())
  {
    bool can_continue = m_advancer_ptr->advance(event_visitor, step_estimate);
    if(!can_continue)
      break;
  }

  m_stream_printer_ptr->write_footer();
}
END_NAMESPACE_CORE