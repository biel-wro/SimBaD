#ifndef LAUNCHER_H
#define LAUNCHER_H
namespace simbad{
namespace cli{

class Launcher{
public:
    Launcher();
    Launcher( Launcher &) = delete;
    Launcher( Launcher &&) = delete;

    Launcher& operator=(Launcher &) = delete;
    Launcher& operator=(Launcher &&) = delete;

    void launch();
};

}
}
#endif
