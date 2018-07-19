#ifndef SIMBAD_CLI_LAUNCHER_H
#define SIMBAD_CLI_LAUNCHER_H

#include "cli_fwd.hpp"
#include "interface/interface_fwd.hpp"

#include <iosfwd>
#include <memory>

BEGIN_NAMESPACE_CLI

class launcher
{
public:
  using property_tree = simbad::core::property_tree;

  launcher(property_tree const &pt);
  ~launcher();

  void launch(property_tree const &pt);

  simbad::core::model &model();

protected:
  void launch_snapshots(property_tree const &pt);
  void launch_final_snapshot(property_tree const &pt);
  void launch_simulation(property_tree const &pt);

  std::unique_ptr<simbad::core::model>
  make_model(property_tree const &pt) const;

  std::unique_ptr<simbad::core::stream_printer>
  make_configuration_printer(std::ostream *os, property_tree const &pt) const;

  std::unique_ptr<simbad::core::configuration_view>
  make_initial_configuration(property_tree const &pt) const;

  void set_initial_configuration(property_tree const &pt);

private:
  std::unique_ptr<simbad::core::model> m_model_ptr;
};

END_NAMESPACE_CLI
#endif
