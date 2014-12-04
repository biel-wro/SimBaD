
#include <boost/program_options.hpp>

#include "core.h"
#include "generic_particle.h"


int hello()
{
    std::cout << "Hello World!" << std::endl;
    return 0;
}

namespace simbad{
namespace core{

void Core::writeModel(const model &model, std::ostream &os) const throw (std::exception)
{
    os << "# model " << model.modelName() << '\n';
    os << "# dimension " << model.dimension() << '\n';
    os << "# time " << model.simulationTime() << '\n';
    os << "# events " << model.simulationEvents() << '\n';
    os << "# particles " << model.particleCount() << '\n';

//    for( GenericParticle p: model ){
//      for( size_t d=0; d<p.dimension(); d++ )
//        {
//            os << p[d] <<" ";
//        }
//        os << '\n';


//    }
}


}}


