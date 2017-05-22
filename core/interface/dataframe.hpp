#ifndef SIMBAD_DATAFRAME_VIEW_HPP
#define SIMBAD_DATAFRAME_VIEW_HPP

#include <functional>
#include "core_fwd.hpp"
BEGIN_NAMESPACE_CORE
class dataframe {
public:
  using data_record = attribute_list;
  using record_visitor = std::function<void(data_record const&)>;

  virtual void visit_records( record_visitor visitor ) const = 0;

  virtual attribute_descriptor const &descriptor() const = 0;
  virtual ~dataframe();
};
END_NAMESPACE_CORE

#endif //SIMBAD_DATAFRAME_VIEW_HPP
