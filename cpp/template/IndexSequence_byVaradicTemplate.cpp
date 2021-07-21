#include <vector>
#include <cstdio>
#include <utility>
#include <array>
#include <iostream>

template <std::size_t... I>
std::vector<std::size_t> make_index_vector(std::index_sequence<I...>) {
    return {(I+1)*(I+1)...};
}

 

template <typename R, typename... Args, typename Container, size_t... I>
R apply_impl(R (*func)(Args...), const Container& c, std::index_sequence<I...>) {
    return (*func)(c[I]...);
}

 

template <typename R, typename... Args, typename Container>
R apply(R (*func)(Args... args), const Container& c) {
    printf("%s %lu\n", __func__, sizeof...(Args));
    return apply_impl(func, c, std::make_index_sequence<sizeof...(Args)>());
}

 

template<typename Ret, typename... Args>
Ret add(Args... args) {
    return (args + ...) ;
}


/*第二种写法
先通过
template<int N, int ...M>
struct make_index_seq: public make_index_seq<N - 1, N -1, M...> 
递归继承（第一次接触继承递归！）


然后通过
template<int ...M>
struct make_index_seq<0, M...> : public index_seq<M...>
终止递归。假设N为10， 终止时最终的继承类型是make_index_seq<0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>， 前者继承自index_seq<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>， 然后将这个模板非类型参数输出了

例如：
make_index_seq<100>首先能匹配template<int N, int ...M> 和 template<int ...M>， 但可能优先匹配template<int N, int ...M>。
然后template<int N, int ...M>继承make_index_seq<N - 1, N -1, M...>,即make_index_seq<99, 99>, 然后make_index_seq<98, 98, 99>, 然后<97, 97, 98, 99>, 一直到<0, 0, 1, 2, 3, 99>。
此时匹配make_index_seq<0, M...>
然后make_index_seq<0, M...>又继承index_seq<M...>

注： 
template<int... N>
struct index_seq{}；
不需要模板类内容， 因为这里目的只是为了去模板非类型参数。
*/

template<int... N>
struct index_seq{};


template<int N, int ...M>
struct make_index_seq: public make_index_seq<N-1, N-1, M...> {

};

template<int ...M>
struct make_index_seq<0, M...> : public index_seq<M...> {
};
 
template <int... N>
void show(index_seq<N...>) {
    (((std::cout << " ") << N), ...);
    std::cout << std::endl;
}

 

int main() {
    using Func = int(*) (int, int, int, int);
    Func func = add;
    using array_type = std::array<int, 10>;
    std::cout << std::tuple_size<array_type>::value << std::endl;
    auto vec = make_index_vector(std::make_index_sequence<std::tuple_size<array_type>::value>{});
    auto sum = apply(func, vec);
    std::cout << sum <<  std::endl;

    show(make_index_seq<10>{});
}
