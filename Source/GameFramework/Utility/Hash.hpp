#pragma once
#include <GameFramework_def.h>

#include <cstdint>

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
struct GAME_FRAMEWORK_API IHashable
{
  virtual ~IHashable() = default;
  virtual size_t Hash() const noexcept = 0;
};
} // namespace GameFramework


namespace std
{
template<typename ObjT>
  requires(std::is_base_of_v<GameFramework::IHashable, ObjT>)
struct hash<ObjT>
{
  std::size_t operator()(const ObjT & obj) const noexcept { return obj.Hash(); }
};
} // namespace std
