#ifndef CORE_H
#define CORE_H

#include "core_global.h"

#include <QMutex>
#include <string>
#include <stdexcept>
#include <memory>

#include "ModelLoader.h"

struct BusyException : public std::logic_error{
    explicit BusyException( std::string const &msg):
        std::logic_error(msg)
    {}
};

class CORESHARED_EXPORT Core
{
public:

    Core() noexcept;

    QStringList getAvailableModels() const noexcept;

    void selectModel( std::string const &modelName ) throw(std::out_of_range,BusyException);
    void start() noexcept;
    void stop() noexcept;


protected:
    enum RUNNING_STATE{
        STOPPED,
        RUNNING,
        PAUSED
    } running;

    QMutex runningMutex;
    ModelLoader modelLoader;

    std::unique_ptr<core::AbstractModel const> pCurrentModel;
};

#endif // CORE_H
