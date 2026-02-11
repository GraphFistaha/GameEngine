#pragma once
#include <GameFramework_def.h>

#include <cstdint>

#include <Utility/Concepts.hpp>

namespace GameFramework::Utils
{

constexpr inline void hash_combine(size_t & seed, size_t hash_value)
{
  seed ^= hash_value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

constexpr inline void hash_combine(size_t & seed, std::initializer_list<size_t> hash_values)
{
  for (auto && hash : hash_values)
    hash_combine(seed, hash);
}

constexpr inline void combined_hash(std::size_t & seed)
{
}

/// @brief combines two hash result
/// @param seed - old hash-value to combine with
/// @param v - value to hash
template<typename T, typename... Rest>
constexpr inline void combined_hash(std::size_t & seed, const T & v, Rest... rest)
{
  std::hash<T> hasher;
  hash_combine(seed, hasher(v));
  combined_hash(seed, rest...);
}

} // namespace GameFramework::Utils

namespace GameFramework
{
template<typename T>
concept Hashable = requires(T obj) {
  { obj.Hash() } -> std::convertible_to<std::size_t>;
};
} // namespace GameFramework


namespace std
{
template<GameFramework::Hashable ObjT>
struct hash<ObjT>
{
  std::size_t operator()(const ObjT & obj) const noexcept { return obj.Hash(); }
};
} // namespace std
