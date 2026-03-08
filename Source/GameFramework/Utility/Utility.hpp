#pragma once

#include <memory>
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

template<typename Derived, typename Base>
  requires(std::is_base_of_v<Base, Derived>)
Derived * FastDynamicCast(Base * base) noexcept
{
#ifdef SAFE_INHERITED_CAST
  return dynamic_cast<Derived *>(base);
#else
  return static_cast<Derived *>(base);
#endif
}

template<typename Derived, typename Base>
  requires(std::is_base_of_v<Base, Derived>)
std::shared_ptr<Derived> FastDynamicCast(std::shared_ptr<Base> base) noexcept
{
#ifdef SAFE_INHERITED_CAST
  return std::dynamic_pointer_cast<Derived>(base);
#else
  return std::static_pointer_cast<Derived>(base);
#endif
}

} // namespace GameFramework::Utils