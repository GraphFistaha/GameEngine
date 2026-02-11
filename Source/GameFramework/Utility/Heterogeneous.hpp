#pragma once
#include <tuple>

#include <Utility/TypeMapping.hpp>

namespace GameFramework::Utils
{

template<typename... Ts>
class Heterogeneous final
{
  using TypeTable = typename type_table<Ts...>;

public:
  explicit Heterogeneous(Ts &&... args)
    : m_fields(std::forward<Ts>(args)...)
  {
  }

  template<typename T>
  decltype(auto) Get() const & noexcept
  {
    return std::get<TypeTable::template index_of<T>>(m_fields);
  }

  template<typename T>
  decltype(auto) Get() & noexcept
  {
    return std::get<TypeTable::template index_of<T>>(m_fields);
  }

private:
  std::tuple<Ts...> m_fields;
};

} // namespace GameFramework::Utils
