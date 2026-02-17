#pragma once
#include <GameFramework_def.h>

#include <array>
#include <optional>
#include <span>
#include <string>

#include <Files/FileStream.hpp>
#include <Utility/Hash.hpp>

namespace GameFramework
{

struct GAME_FRAMEWORK_API Uuid final
{
  Uuid();
  explicit Uuid(std::span<const char, 16> bytes16);
  explicit Uuid(std::span<const std::byte, 16> bytes16);
  ~Uuid();

  bool operator==(const Uuid & rhs) const noexcept;
  bool operator!=(const Uuid & rhs) const noexcept;
  bool operator<(const Uuid & rhs) const noexcept;

  operator bool() const noexcept { return !IsNull(); }
  bool IsNull() const noexcept;

  size_t Hash() const noexcept;
  std::string ToString() const noexcept;

  static std::optional<Uuid> MakeFromString(const std::string_view & str);
  static std::optional<Uuid> MakeFromString(const std::wstring_view & str);
  static Uuid MakeRandomUuid();

public:
  static size_t ReadBinary(IBinaryFileReader & stream, Uuid & uuid);
  static void WriteBinary(IBinaryFileWriter & stream, const Uuid & uuid);

private:
  std::byte m_bytes[16];

public:
};

static_assert(Hashable<Uuid>);

} // namespace GameFramework
