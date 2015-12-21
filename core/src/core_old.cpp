#include "core_old.h"
#include <QMutexLocker>

Core::Core() noexcept:
    running( STOPPED ),
    pCurrentModel( nullptr )
{
    modelLoader.loadPlugins();
}

QStringList Core::getAvailableModels() const noexcept
{
    return modelLoader.modelsNames();
}

void Core::selectModel(const std::string &modelName) throw(std::out_of_range,BusyException)
{
    {
        QMutexLocker locker(&runningMutex);
        if( running != STOPPED )
        {
            throw BusyException("can't change model while curent model is not stopped");
        }

    }

    //pCurrentModel = std::move( modelLoader.getFactory(modelName).createModel( ) );
}
