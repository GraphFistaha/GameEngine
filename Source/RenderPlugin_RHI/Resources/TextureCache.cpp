#include "TextureCache.hpp"

#include <Files/FileManager.hpp>
#include <Game/Async.hpp>

namespace RenderPlugin
{
TextureCache::TextureCache(RHI::IContext & ctx)
  : GameFramework::OwnedBy<RHI::IContext>(ctx)
{
}

std::shared_ptr<GameFramework::IAssetData> TextureCache::LoadBase(
  const GameFramework::Asset * asset, bool async)
{
  if (auto tex = GetBase(asset))
  {
    return tex;
  }

  auto [it, inserted] = m_textures.insert({asset->GetUUID(), nullptr});
  assert(inserted);
  it->second = std::make_shared<Texture2D>();

  auto uploadTextureTask = [path = asset->GetPath(), texture = it->second, this]
  {
    if (auto file = GameFramework::GetFileManager().OpenReadBinary(path))
    {
      texture->ReadBinary(*file, *texture);
      auto [it, inserted] = m_gpuTextures.insert({texture->GetFormat(), nullptr});
      if (inserted)
      {
        RHI::TextureDescription args{};
        args.type = RHI::ImageType::Image2D_Array;
        args.extent = texture->GetFormat().GetExtent();
        args.format = texture->GetFormat().GetInternalImageFormat();
        args.layersCount = 128;
        args.mipLevels = RHI::CalcMaxMipLevels(args.extent);
        it->second = GetContext().CreateTexture(args);
      }
      texture->BindToGpuContainer(it->second, 0);
    }
  };

  if (async)
  {
    GameFramework::GetAsyncWorker().ScheduleTask(std::move(uploadTextureTask));
  }
  else
  {
    uploadTextureTask();
  }

  return it->second->IsReadyToUse() ? it->second : nullptr;
}


std::shared_ptr<GameFramework::IAssetData> TextureCache::GetBase(
  const GameFramework::Asset * asset) const noexcept
{
  if (auto it = m_textures.find(asset->GetUUID()); it != m_textures.end())
  {
    return it->second->IsReadyToUse() ? it->second : nullptr;
  }
  return nullptr;
}

} // namespace RenderPlugin
