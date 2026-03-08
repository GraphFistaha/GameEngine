#pragma once
#include <GameFramework_def.h>

#include <Assets/AssetsRegistry.hpp>

namespace GameFramework::details
{
AssetType GetAssetTypeByPath(const std::filesystem::path & path) noexcept;

GAME_FRAMEWORK_API std::wstring_view AssetTypeToWString(AssetType type) noexcept;
GAME_FRAMEWORK_API std::string_view AssetTypeToString(AssetType type) noexcept;
AssetType StringToAssetType(std::wstring_view str) noexcept;

} // namespace GameFramework::details
