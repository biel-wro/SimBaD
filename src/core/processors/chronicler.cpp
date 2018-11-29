#include "chronicler.hpp"

#include "interface/advance_estimator.hpp"
#include "interface/stream_printer.hpp"
#include "processors/advancer.hpp"
#include "processors/chronicles_builder.hpp"

BEGIN_NAMESPACE_CORE
struct chronicler::builder : public chronicles_builder
{
  chronicler &m_chronicler;
  void emit_particle(tracker_record const &record, chronicle_datum const &datum,
                     double death_time) overwrite
  {
  }
};

chronicler::~chronicler() = default;

void chronicler::launch()
{
  if(nullptr == m_final_estimator)
    launch_without_estimator();
  else
    launch_with_estimator();
}

void chronicler::launch_without_estimator()
{
  bool can_continue = true;

  auto event_visitor = [this](attribute_list const &event) {
    m_builder_ptr->push_event(event);
  };

  do
    can_continue =
        m_advancer_ptr->advance(event_visitor, default_advancement_step);
  while(can_continue);
}

END_NAMESPACE_CORE