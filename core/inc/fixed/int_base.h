#ifndef INTBASE_H
#define INTBASE_H

#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>
namespace core {
namespace fixed{

template<int bits, bool signed_=true>
struct int_base{
    static_assert(true,"base of gived size is not available");
};


template<>
struct int_base<32,true>{
    typedef std::int32_t type;
};

template<>
struct int_base<32,false>{
    typedef std::uint32_t type;
};

template<>
struct int_base<64,true>{
    typedef std::int64_t type;
};

template<>
struct int_base<64,false>{
    typedef std::uint64_t type;
};

template<>
struct int_base<128,true>{
    typedef __int128_t type;
};

}
}
#endif // INTBASE_H
