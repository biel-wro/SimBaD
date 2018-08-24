#include "cbor_lite_io.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_kind.hpp"
#include "interface/attribute_list.hpp"
#include "interface/property_tree.hpp"
#include "interface/stream_printer.hpp"
#include "interface/stream_reader.hpp"
#include "repositories/factory_registration.hpp"

#include <cbor-lite/codec.h>

#include <boost/lexical_cast.hpp>

#include <exception>
#include <iostream>
#include <iterator>

namespace simbad
{
namespace plugins
{
namespace cbor_lite_io
{
namespace
{
enum class PRECISION
{
  DOUBLE,
  SINGLE
}; // half not supported by cbor-lite

static PRECISION precision_from_string(std::string const &val)
{
  if("double" == val)
    return PRECISION::DOUBLE;
  if("single" == val)
    return PRECISION::SINGLE;
  throw std::invalid_argument("unrecognized precision `" + val + "`");
}

class cbor_printer : public core::stream_printer
{
public:
  explicit cbor_printer(core::property_tree const &pt)
      : core::stream_printer(pt.get("file", "STDIN")),
        m_precision(precision_from_string(pt.get("precision", "double")))
  {
  }

  void write_header(core::attribute_description const &description) override
  {
    m_indices.clear();
    std::string buffer;

    CborLite::encodeArraySize(buffer, description.size());
    for(core::attribute_descriptor const &d : description)
    {
      m_indices.push_back(d.attribute_idx());
      CborLite::encodeArraySize(buffer, std::size_t(4));
      {
        CborLite::encodeText(buffer, d.attribute_name());
        CborLite::encodeInteger(buffer, d.attribute_dimension());
        CborLite::encodeText(buffer,
                             boost::lexical_cast<std::string>(d.kind()));
        CborLite::encodeText(buffer,
                             boost::lexical_cast<std::string>(d.scalar()));
      }
    }
    ostream() << buffer;
  };

  template <PRECISION precision> struct attribute_writer
  {
    explicit attribute_writer(std::string &buffer) : m_buf(buffer) {}

    void operator()(std::string const &s) const
    {
      CborLite::encodeText(m_buf, s);
    }
    void operator()(double val) const
    {
      if(PRECISION::DOUBLE == precision)
        CborLite::encodeDoubleFloat(m_buf, val);
      else if(PRECISION::SINGLE == precision)
        CborLite::encodeSingleFloat(m_buf, val);
    }
    void operator()(std::int64_t val) const
    {
      CborLite::encodeInteger(m_buf, val);
    }

    template <class Scalar>
    void operator()(core::array_attribute<Scalar> const &array) const
    {
      CborLite::encodeArraySize(m_buf, array.size());
      for(Scalar const &val : array)
        (*this)(val);
    }

    template <class Scalar, std::size_t dim>
    void operator()(core::coordinates<Scalar, dim> const arr) const
    {
      CborLite::encodeArraySize(m_buf, dim);
      for(Scalar const &val : arr)
        (*this)(val);
    };
    std::string &m_buf;
  };
  template <PRECISION precision>
  void write_entry_with(core::attribute_list const &entry)
  {
    std::string buffer;

    attribute_writer<precision> writer(buffer);

    CborLite::encodeArraySize(buffer, m_indices.size());
    for(std::size_t attribute_index : m_indices)
    {
      core::attribute attr = entry[attribute_index];
      boost::apply_visitor(writer, attr);
    }
    ostream() << buffer;
  }
  void write_entry(core::attribute_list const &entry) override
  {
    if(PRECISION::DOUBLE == m_precision)
      write_entry_with<PRECISION::DOUBLE>(entry);
    else if(PRECISION::SINGLE == m_precision)
      write_entry_with<PRECISION::SINGLE>(entry);
  }

  void write_footer() override {}

private:
  PRECISION const m_precision;
  std::vector<std::size_t> m_indices;
};
}
/*
class cbor_reader : public core::stream_reader
{
public:
  core::attribute_description read_header() override
  {
    std::istream_iterator<char> eos;
    std::istream_iterator<char> it(istream());

    std::size_t record_length;
    CborLite::decodeArraySize(it, eos, record_length);
    m_descriptors.resize(record_length);
    m_last_record.resize(record_length);

    core::attribute_description description;
    for(std::size_t idx = 0; idx < record_length; ++idx)
    {
      std::size_t descriptor_length;
      CborLite::decodeArraySize(it, eos, descriptor_length);
      if(descriptor_length != 4)
        throw std::runtime_error("bad description");

      std::string name;
      CborLite::decodeText(it, eos, name);

      std::size_t dimension;
      CborLite::decodeInteger(it, eos, dimension);

      std::string kind_str;
      CborLite::decodeText(it, eos, kind_str);
      core::ATTRIBUTE_KIND attribute_kind =
          boost::lexical_cast<core::ATTRIBUTE_KIND>(kind_str);

      std::string scalar_str;
      CborLite::decodeText(it, eos, scalar_str);
      core::ATTRIBUTE_SCALAR attribute_scalar =
          boost::lexical_cast<core::ATTRIBUTE_SCALAR>(scalar_str);

      core::attribute_descriptor descriptor{
          idx, std::move(name), attribute_kind, attribute_scalar, dimension};
      description.add_attribute(descriptor);
    }

    return description;
  }

  void visit_entries(entry_visitor v, std::size_t max_reads = 0) override {}

private:
  std::vector<core::attribute_descriptor> m_descriptors;
  std::vector<core::attribute> m_last_record;
};
*/
void initialize_plugin()
{
  core::register_creator<cbor_printer>(
      core::factory<core::stream_printer>::global_instance_mutable(),
      "cbor_lite");
}
} // namespace cbor_lite
} // namespace plugins
} // namespace simbad