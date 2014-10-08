#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <exception>

#include "model.h"

namespace simbad{
namespace core{

class Core{

    void writeModel(Model const &m, std::ostream & os) const throw(std::exception);
};

}
}
int hello();
#endif
