#pragma once
#include <unordered_map>

#include <Assets/AssetCache.hpp>
#include <Resources/Texture2D.hpp>
#include <Utility/OwnedBy.hpp>

namespace RenderPlugin
{
struct TextureCache : public GameFramework::IAssetCache,
                      public GameFramework::OwnedBy<RHI::IContext>
{
  using CachedAsset = Texture2D;
  explicit TextureCache(RHI::IContext & ctx);
  MAKE_ALIAS_FOR_GET_OWNER(RHI::IContext, GetContext);

private: // IAssetCache
  virtual std::shared_ptr<GameFramework::IAssetData> LoadBase(const GameFramework::Asset * asset,
                                                              bool async = true) override;

  virtual std::shared_ptr<GameFramework::IAssetData> GetBase(
    const GameFramework::Asset * asset) const noexcept override;

private:
  std::unordered_map<GameFramework::Uuid, std::shared_ptr<Texture2D>> m_textures;
  //TODO: select more efficient container for textures (array 2d/3d where i is a pow of 2)
  std::unordered_map<TextureFormat, RHI::ITexture *> m_gpuTextures;
};
} // namespace RenderPlugin
