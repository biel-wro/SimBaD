#include "model_factory.hpp"


BEGIN_NAMESPACE_CORE

std::string model_factory::class_name() const{
    return model_name();
}

model_factory::model_factory() {}

model_factory::~model_factory() {}

END_NAMESPACE_CORE
