#pragma once
#include <memory>
#include <unordered_map>

#include <Assets/AssetCache.hpp>
#include <Resources/Material.hpp>

namespace RenderPlugin
{

struct MaterialCache : public GameFramework::IAssetCache
{
  using CachedAsset = Material;

  MaterialCache();

private: // IAssetCache
  virtual std::shared_ptr<GameFramework::IAssetData> LoadBase(const GameFramework::Asset * asset,
                                                              bool async = true) override;

  virtual std::shared_ptr<GameFramework::IAssetData> GetBase(
    const GameFramework::Asset * asset) const noexcept override;

private:
  std::unordered_map<GameFramework::Uuid, std::shared_ptr<Material>> m_materials;
  std::shared_ptr<GameFramework::IAssetData> m_nullMaterial;
};

} // namespace RenderPlugin
