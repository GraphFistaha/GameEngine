#pragma once
#include <unordered_map>

#include <Assets/AssetCache.hpp>
#include <Resources/ShaderFile.hpp>

namespace RenderPlugin
{

struct ShadersCache : public GameFramework::IAssetCache
{
  using CachedAsset = ShaderFile;

  ShadersCache();

private: // IAssetCache
  virtual std::shared_ptr<GameFramework::IAssetData> LoadBase(const GameFramework::Asset * asset,
                                                              bool async = true) override;

  virtual std::shared_ptr<GameFramework::IAssetData> GetBase(
    const GameFramework::Asset * asset) const noexcept override;

private:
  std::unordered_map<GameFramework::Uuid, std::shared_ptr<ShaderFile>> m_shaders;
  std::shared_ptr<GameFramework::IAssetData> m_nullShader;
};

} // namespace RenderPlugin
