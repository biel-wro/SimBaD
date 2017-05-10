#ifndef SIMBAD_FINITE_DATAFRAME_HPP
#define SIMBAD_FINITE_DATAFRAME_HPP

#include "core_fwd.hpp"

#include "dataframe_view.hpp"
BEGIN_NAMESPACE_CORE
class finite_dataframe : public dataframe_view{
public:
  virtual std::size_t size() const = 0;
  ~finite_dataframe() override;
};
END_NAMESPACE_CORE

#endif //SIMBAD_FINITE_DATAFRAME_HPP
