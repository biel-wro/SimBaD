#ifndef SIMBAD_PRINTING_CHRONICLE_BUILDER_HPP
#define SIMBAD_PRINTING_CHRONICLE_BUILDER_HPP

#include "processors_fwd.hpp"

#include "interface/dataframe.hpp"
#include "processors/chronicles_builder.hpp"

#include <cstddef>

BEGIN_NAMESPACE_CORE
class printing_chronicle_builder : public chronicles_builder
{
public:
  static constexpr std::size_t extra_attributes_offset = 4;

  printing_chronicle_builder(
      attribute_description const &event_description,
      std::string const &key_name,
      std::vector<std::string> const &nonkey_attribute_names,
      stream_printer &printer);

  attribute_description chronicle_description() const;


  void write_header();
  void emit_particle(tracker_record const &record, chronicle_datum const &datum,
                     double death_time) override;
  void write_footer();

protected:
private:

  struct chronicle_record;
  stream_printer &m_printer;
};
END_NAMESPACE_CORE
#endif // SIMBAD_PRINTING_CHRONICLE_BUILDER_HPP
