//只是起到展开表达式的作用。
//最终结果看表达式展开如何计算， 注意此时，不要将运算符的优先级与计算顺序混淆。具体见operatorPreference_VS_orderOfEvaluation.cpp
//1. 如果省略号在parameter pack右边即右展开， 即parameter pack里最右边元素最先展开，即(E1 Op (...(En-2 Op（En Op En-1))))
//   如果省略号在左， 那么最左侧元素最先展开， 即((((E1 op E2) op E3) op ...) op En)
#include <iostream>
#include <vector>
#include <climits>
#include <cstdint>
#include <type_traits>
#include <utility>
 
template<typename ...Args>
void printer(Args&&... args) {
    (std::cout << ... << args) << '\n';
}

/*
template<typename ...Args>
void printer2(Args&&... args) {
    (std::cout << args << ...) << '\n';
}
*/

//same result for multiple
template<typename ...Args>
auto left_multiple(Args... args) {
  return (... * args);
}
template<typename ...Args>
auto right_multiple(Args... args) {
  return (args * ...);
}
 
//difference result divide
template<typename ...Args>
auto left_divide(Args... args) {
  return (... / args);
}
template<typename ...Args>
auto right_divide(Args... args) {
  return (args / ...);
}

//same result for comma
template<typename T, typename... Args>
void right_push_back(std::vector<T>& v, Args&&... args)
{
    static_assert((std::is_constructible_v<T, Args&&> && ...));
    //(v.push_back(std::forward<Args>(args)), ...);
    (v.push_back(args), ...);
}
template<typename T, typename... Args>
void left_push_back_2(std::vector<T>& v, Args&&... args)
{
    static_assert((std::is_constructible_v<T, Args&&> && ...));
    //(v.push_back(std::forward<Args>(args)), ...);
    (..., v.push_back(args));
}


 //下面没看了
// compile-time endianness swap based on http://stackoverflow.com/a/36937049 
template<class T, std::size_t... N>
constexpr T bswap_impl(T i, std::index_sequence<N...>) {
  return (((i >> N*CHAR_BIT & std::uint8_t(-1)) << (sizeof(T)-1-N)*CHAR_BIT) | ...);
}
template<class T, class U = std::make_unsigned_t<T>>
constexpr U bswap(T i) {
  return bswap_impl<U>(i, std::make_index_sequence<sizeof(T)>{});
}
 
int main()
{
    printer(1, 2, 3, "abc");

    std::cout << left_multiple(1000, 2, 10, 0.5) << std::endl; //  (((1000 * 2) * 10) * 0.5)
    std::cout << right_multiple(1000, 2, 10, 0.5) << std::endl; // (1000 * (2 * (10 * 0.5)))
    std::cout << left_divide(1000, 2, 10, 0.5) << std::endl;  //   (((1000 / 2) / 10) / 0.5)
    std::cout << right_divide(1000, 2, 10, 0.5) << std::endl; //   (1000 / (2 / (10 / 0.5)))

    std::vector<int> v;
    right_push_back(v, 6, 2, 45, 12);       //(v.push_back(6), (v.push_back(2), (v.push_back(45), v.push_back(12))))
    for (int i : v) std::cout << i << ' ';
    std::cout << std::endl;
    std::vector<int> v2;
    left_push_back_2(v2, 6, 2, 45, 12);     //(((v.push_back(6), v.push_back(2)), v.push_back(45)), v.push_back(12)) 
    for (int i : v2) std::cout << i << ' ';
    std::cout << std::endl;


 
    static_assert(bswap<std::uint16_t>(0x1234u)==0x3412u);
    static_assert(bswap<std::uint64_t>(0x0123456789abcdefULL)==0xefcdab8967452301ULL);
}