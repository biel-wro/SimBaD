#ifndef PARTICLEWRAPPER_H
#define PARTICLEWRAPPER_H

#include <assert.h>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>

#include "particle_interface.h"

namespace simbad {
namespace core {

template< class BaseParticle, class ... CoordGetter >
class ParticleWrapper : public ParticleInterface {

    typedef ParticleWrapper<BaseParticle, CoordGetter...> type;
    static const constexpr size_t DIM = sizeof...(CoordGetter);
    typedef boost::mpl::vector<CoordGetter...> GetterVector;

    struct Retreiver{
        Retreiver( BaseParticle const &particleRef, long target_idx, double &result) :
            particleRef(particleRef),
            target_idx(target_idx),
            result(result)
        {}
        double getResult(){ return result; }
    private:
        BaseParticle const &particleRef;
        long target_idx;
        double &result;
    public:
        template<class G>
        void operator()(G const &g){
            if( target_idx == 0 )
                result = g(particleRef);
            target_idx--;
        }
    };

private:
    BaseParticle const &base;

public:
    ParticleWrapper( BaseParticle const &base, CoordGetter const &... ) :
        base(base)
    {}

    size_t dimension() const{
        return DIM;
    }

    double coord(size_t d) const{
        assert(d<DIM);
        double result;
        Retreiver retreiver(base, d,result);
        boost::mpl::for_each<GetterVector>( retreiver );
        return result;

    }

    size_t particleType() const {
        return 0;
    }

    std::unique_ptr<ParticleInterface> clone() const{
        ParticleInterface *ptr = new ParticleWrapper( base, CoordGetter()... );
        return std::unique_ptr<ParticleInterface>(ptr);
    }
};

}
}
#endif // PARTICLECONVERTER_H
