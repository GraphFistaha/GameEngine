#pragma once

#include <filesystem>

#include <Utility/Hash.hpp>
#include <Utility/Uuid.hpp>

namespace GameFramework
{
enum class AssetType
{
  Unknown,
  Text,          //.txt, .md
  Config,        //.json, .ini,
  Picture,       // .bmp, .png, jpg, dds, tga
  Audio,         // wav, mp3, ogg,
  ShaderSource,  // glsl, hlsl, vert, frag, geom
  ShaderBinary,  // spv, dxil, cso
  ShaderInclude, // inc, hlsli, glsli
  Material,      // mat
};

struct GAME_FRAMEWORK_API Asset final
{
  explicit Asset(AssetType type, const Uuid & uuid, const std::filesystem::path & path);
  ~Asset() noexcept;
  Asset(Asset && rhs) noexcept;
  Asset & operator=(Asset && rhs) noexcept;
  Asset(const Asset & rhs);
  Asset & operator=(const Asset & rhs);

public:
  Uuid GetUUID() const noexcept;
  std::filesystem::path GetPath() const noexcept;
  AssetType GetType() const noexcept;

private:
  struct Impl;
  std::byte m_impl[64];
};
using AssetUPtr = std::unique_ptr<Asset>;


struct IAssetData
{
  virtual ~IAssetData() = default;
  virtual bool IsReadyToUse() const noexcept = 0;
};
} // namespace GameFramework
