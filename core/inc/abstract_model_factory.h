#ifndef MODELFACTORYTEMPLATE_H
#define MODELFACTORYTEMPLATE_H

#include <QString>

#include "model_factory.h"
namespace simbad {
namespace core {

template<class Model>
class model_factory_template : public model_factory{

public:
    model_factory_template() noexcept {}
    ~model_factory_template() noexcept{}

    std::string model_name() const noexcept override = 0;
    std::unique_ptr<model>
    createModel(po::variables_map const &args ) const noexcept override
    {
        model *pModel = new Model(*this,args);
        std::unique_ptr<AbstractModel> ptr = std::unique_ptr<AbstractModel>( pModel);
        return ptr;
    }

};




}}

#endif
