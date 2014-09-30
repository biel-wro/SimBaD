#ifndef CONVERSION_H
#define CONVERSION_H
#include <stddef.h>
#include <type_traits>

namespace core{
namespace fixed{

constexpr size_t offsetToMult(unsigned offset){
    return offset==0? 1 : 2*offsetToMult(offset-1);
}

template< int offset, class integer>
typename std::enable_if<std::is_integral<integer>::value,
integer>::type
offset_shift(integer const &v){
    static_assert( offset < sizeof(integer)*8,"offset must be smaller than type width");
    return offset>0 ? v << offset : v >> offset;
}

template< int offset, class integer>
typename std::enable_if<std::is_integral<integer>::value,
integer>::type
offset_unshift(integer const &v){
    static_assert( offset < sizeof(integer)*8,"offset must be smaller than type width");
    return offset>0 ? v >> offset : v << offset;
}

struct num_to_int{

    template<int offset, class target, class src>
    typename std::enable_if< std::is_floating_point<src>::value,
    target>::type
    operator()(src num) const{
          return offset>0 ? num * offsetToMult(offset) : num / offsetToMult(offset);
    }

    template<int offset, class target, class src>
    typename std::enable_if<std::is_integral<src>::value,
    typename std::enable_if<(sizeof(target) <= sizeof(src) ),
    target>::type >::type
    operator()(src num) const{
        return offset_shift<offset>(num);
    }

    template<int offset, class target, class src>
    typename std::enable_if<std::is_integral<src>::value,
    typename std::enable_if<(sizeof(target) > sizeof(src) ),
    target>::type >::type
    operator()(src num) const{
        return offset_shift<offset>( target(num) );
    }
};

//template< class numeric, int offset>
struct int_to_num{

    template<int offset, class target, class src>
    typename std::enable_if< std::is_floating_point<target>::value,
    target>::type
    operator()(src repr) const{
        return offset>0 ? repr / offsetToMult(offset) : repr * offsetToMult(offset);
    }

    template<int offset, class target, class src>
    typename std::enable_if<std::is_integral<target>::value,
    typename std::enable_if<(sizeof(target) <= sizeof(src) ),
    target>::type >::type
    operator()(src num) const{
        return offset_unshift<offset>(num);
    }

    template<int offset, class target, class src>
    typename std::enable_if<std::is_integral<target>::value,
    typename std::enable_if<(sizeof(target) > sizeof(src) ),
    target>::type >::type
    operator()(src num) const{
        return offset_unshift<offset>( target(num) );
    }
};

}}
#endif // UTILS_H
