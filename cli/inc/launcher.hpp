#ifndef LAUNCHER_H
#define LAUNCHER_H
#include "cli_fwd.hpp"

BEGIN_NAMEMESPACE_CLI

class Launcher{
public:
    Launcher();

    Launcher( Launcher &) = delete;
    Launcher( Launcher &&) = delete;

    Launcher& operator=(Launcher &) = delete;
    Launcher& operator=(Launcher &&) = delete;

    void launch();
};

END_NAMESPACE_CLI
#endif
