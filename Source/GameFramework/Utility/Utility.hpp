#pragma once

#include <utility>

namespace GameFramework::Utils
{

// internal overload with indices tag
template<typename InIt, std::size_t... Idxs,
         typename ValueType = typename std::iterator_traits<InIt>::value_type,
         typename Array = std::array<ValueType, sizeof...(Idxs)>>
constexpr Array make_array(InIt first, std::index_sequence<Idxs...>)
{
  return Array{{(void(Idxs), *first++)...}};
}

// externally visible interface
template<std::size_t N, typename InIt,
         typename ValueType = typename std::iterator_traits<InIt>::value_type>
constexpr std::array<ValueType, N> make_array(InIt first, InIt last)
{
  // last is not relevant if we're assuming the size is N
  // I'll assert it is correct anyway
  static_assert(last - first == N);
  return make_array(first, std::make_index_sequence<N>{});
}

template<typename T>
constexpr T bit(T i)
{
  return static_cast<T>(1) << i;
}

// A simple visitor using overloaded lambdas
template<class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace GameFramework::Utils
