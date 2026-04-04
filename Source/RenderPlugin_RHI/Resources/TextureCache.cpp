#include "TextureCache.hpp"

#include <Assets/AssetsRegistry.hpp>
#include <Files/FileManager.hpp>
#include <Game/Async.hpp>

namespace RenderPlugin
{
TextureCache::TextureCache(RHI::IContext & ctx)
  : GameFramework::OwnedBy<RHI::IContext>(ctx)
{
  auto * nullTexture = GameFramework::GetAssetsRegistry().GetAsset("Textures/NullTexture.png");
  m_nullTexture = LoadBase(nullTexture, false);
}

std::shared_ptr<GameFramework::IAssetData> TextureCache::LoadBase(
  const GameFramework::Asset * asset, bool async)
{
  if (!asset)
    return m_nullTexture;

  auto [it, inserted] = m_textures.insert({asset->GetUUID(), nullptr});
  if (inserted)
  {
    it->second = std::make_shared<Texture2D>();

    auto uploadTextureTask = [path = asset->GetPath(), texture = it->second, this]
    {
      if (auto file = GameFramework::GetFileManager().OpenReadBinary(path))
      {
        if (!Texture2D::ReadBinary(*file, *texture))
          throw std::runtime_error("Failed to read metadata of texture");

        auto [it, inserted] = m_gpuTextures.insert({texture->GetFormat(), nullptr});
        if (inserted)
        {
          RHI::TextureDescription args{};
          args.type = RHI::ImageType::Image2D_Array;
          args.extent = texture->GetFormat().GetExtent();
          args.extent[2] = 128;
          args.format = texture->GetFormat().GetInternalImageFormat();
          args.mipLevels = RHI::CalcMaxMipLevels(args.extent);
          it->second =
            std::make_unique<RHI::DynamicContainers::TexturePalette>(&GetContext(), args);
        }
        auto textureIndex = it->second->Alloc(texture->GetFormat().GetExtent());
        texture->BindToGpuContainer(it->second.get(), textureIndex);
        if (!Texture2D::ReadBinary(*file, *texture))
          throw std::runtime_error("Failed to load texture");
      }
    };

    if (async)
    {
      GameFramework::GetAsyncWorker().ScheduleTask(std::move(uploadTextureTask));
    }
    else
    {
      uploadTextureTask();
      GetContext().TransferPass();
      GetContext().TransferPass(); // TODO: make a flush
    }
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

std::vector<RHI::ISamplerUniformDescriptor *> TextureCache::BindToPipeline(
  RHI::ISubpassConfiguration * pipelineConfig, RHI::LayoutIndex layoutIndex,
  RHI::ShaderType shaderTypeMask) const
{
  std::vector<RHI::ISamplerUniformDescriptor *> result(m_gpuTextures.size(), nullptr);
  pipelineConfig->DeclareSamplersArray(layoutIndex, shaderTypeMask,
                                       static_cast<uint32_t>(m_gpuTextures.size()), result.data());

  UpdatePipeline(result);
  return result;
}

void TextureCache::UpdatePipeline(
  const std::vector<RHI::ISamplerUniformDescriptor *> & samplers) const
{
  assert(samplers.size() == m_gpuTextures.size());
  auto it = m_gpuTextures.begin();
  for (auto * descriptor : samplers)
  {
    descriptor->AssignImage(it->second->Handle());
    it++;
  }
}

size_t TextureCache::PaletteHash() const noexcept
{
  size_t seed = 0;
  for (auto && [format, palette] : m_gpuTextures)
    GameFramework::Utils::hash_combine(seed, palette->TextureHash());
  return seed;
}

} // namespace RenderPlugin
