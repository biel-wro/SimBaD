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
  void load_model();
  void load_replay();
  void load_reader();

  void set_initial_configuration(core::configuration_reader &);

  void launch_model();
  void launch_nonmodel();

  void launch_snapshots(core::model &, property_tree const &pt);
  void launch_stream(core::model &, property_tree const &pt);
  void launch_final_snapshot(core::model &, property_tree const &pt);

  void launch_chronicler(core::stream_reader &, property_tree const &pt);

private:
  property_tree m_property_tree;

  std::unique_ptr<core::stream_reader> m_stream_reader_ptr;
  std::unique_ptr<core::model> m_model_ptr;
};

END_NAMESPACE_CLI
#endif
