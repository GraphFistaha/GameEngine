#pragma once
#include <GameFramework_def.h>

#include <Assets/Asset.hpp>
#include <Files/FileStream.hpp>

namespace GameFramework
{

struct GAME_FRAMEWORK_API AssetSlot final
{
  AssetSlot(AssetType type);
  ~AssetSlot();

public:
  void SetAsset(const Uuid & uuid);
  void SetAsset(const std::filesystem::path & path);
  void ClearAsset();
  const IAsset * GetAsset() const noexcept;

public:
  static size_t ReadBinary(IBinaryFileReader & stream, AssetSlot & slot);
  static void WriteBinary(IBinaryFileWriter & stream, const AssetSlot & slot);

private:
  AssetType m_assetType;
  IAsset * m_asset = nullptr;
};

} // namespace GameFramework
