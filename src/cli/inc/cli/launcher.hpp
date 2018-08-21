#ifndef SIMBAD_CLI_LAUNCHER_H
#define SIMBAD_CLI_LAUNCHER_H

#include "cli_fwd.hpp"
#include "interface/interface_fwd.hpp"
#include "interface/property_tree.hpp"

#include <iosfwd>
#include <memory>

BEGIN_NAMESPACE_CLI

class launcher
{
public:
  using property_tree = core::property_tree;


  explicit launcher(property_tree pt);
  ~launcher();

  void launch();

protected:
  void launch_snapshots(property_tree const &pt);
  void launch_stream(property_tree const &pt);
  /*
   void launch_final_snapshot(property_tree const &pt);
   void launch_simulation(property_tree const &pt);
 */
private:
  property_tree m_property_tree;
  std::unique_ptr<core::model> m_model_ptr;
};

END_NAMESPACE_CLI
#endif
