#pragma once
#include <GameFramework_def.h>

#include <filesystem>
#include <optional>

#include <Assets/Asset.hpp>
#include <Utility/Uuid.hpp>

namespace GameFramework
{

static constexpr std::wstring_view s_assetsModuleFilename = L".assetList";

/// Total data base of all assets in application
struct AssetsRegistry
{
  virtual ~AssetsRegistry() = default;

  /// @brief deletes database from memory
  virtual void DeleteDatabase(const std::filesystem::path & folder) = 0;

  /// @brief loads entire database of meta-assets to RAM
  /// @param folder with assets (must contain .assetList)
  virtual void LoadDatabase(const std::filesystem::path & folder) = 0;

  /// @brief get asset by uuid
  virtual const Asset * GetAsset(const Uuid & uuid) const = 0;

  /// @brief get asset by path
  virtual const Asset * GetAsset(const std::filesystem::path & path) const = 0;
};

using AssetsRegisryUPtr = std::unique_ptr<AssetsRegistry>;

GAME_FRAMEWORK_API AssetsRegistry & GetAssetsRegistry();

} // namespace GameFramework
