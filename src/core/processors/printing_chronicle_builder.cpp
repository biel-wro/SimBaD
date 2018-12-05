#include "processors/printing_chronicle_builder.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_kind.hpp"
#include "interface/attribute_list.hpp"
#include "interface/stream_printer.hpp"
#include "interface/attribute.hpp"

BEGIN_NAMESPACE_CORE

struct printing_chronicle_builder::chronicle_record : public attribute_list
{
  chronicle_record(tracker_record const &rec, chronicle_datum const &dat,
                   double const &death)
      : record(rec), datum(dat), death_time(death)
  {
  }
  ~chronicle_record() = default;
  attribute get_attribute(std::size_t idx) const
  {
    switch(idx)
    {
    case 0: return datum.id;
    case 1: return datum.parent_id;
    case 2: return datum.birth_time;
    case 3: return death_time;
    default: return record.get(idx - extra_attributes_offset);
    }
  }
  tracker_record const &record;
  chronicle_datum const &datum;
  double const &death_time;
};

printing_chronicle_builder::printing_chronicle_builder(
    attribute_description const &event_description, std::string const &key_name,
    std::vector<std::string> const &nonkey_attribute_names,
    stream_printer &printer)
    : chronicles_builder(event_description, key_name, nonkey_attribute_names),
      m_printer(printer)
{
}

attribute_description printing_chronicle_builder::chronicle_description() const
{
  attribute_description result;

  result.add_attribute_auto_idx("id",                         //
                                ATTRIBUTE_KIND::PARTICLE_UID, //
                                ATTRIBUTE_SCALAR::INT,        //
                                1);

  result.add_attribute_auto_idx("parent.id",                //
                                ATTRIBUTE_KIND::PARENT_UID, //
                                ATTRIBUTE_SCALAR::INT,      //
                                1);

  result.add_attribute_auto_idx("birth_time",               //
                                ATTRIBUTE_KIND::EVENT_TIME, //
                                ATTRIBUTE_SCALAR::REAL,     //
                                1);

  result.add_attribute_auto_idx("death_time",               //
                                ATTRIBUTE_KIND::EVENT_TIME, //
                                ATTRIBUTE_SCALAR::REAL,     //
                                1);

  result.add_and_map_attributes(m_dataframe_description);

  return result;
}

void printing_chronicle_builder::write_header()
{
  m_printer.write_header(chronicle_description());
}

void printing_chronicle_builder::emit_particle(const tracker_record &record,
                                               const chronicle_datum &datum,
                                               double death_time)
{
  chronicle_record rec{record, datum, death_time};
  m_printer.write_entry(rec);
}

void printing_chronicle_builder::write_footer() { emit_alive(); }

END_NAMESPACE_CORE
