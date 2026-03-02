#include "ShadersCache.hpp"

#include <Assets/AssetsRegistry.hpp>
#include <Files/FileManager.hpp>
#include <Game/Async.hpp>
#include <Resources/ShaderFile.hpp>

namespace RenderPlugin
{
ShadersCache::ShadersCache()
{
  auto * nullShader = GameFramework::GetAssetsRegistry().GetAsset("Shaders/NullShader.frag");
  m_nullShader = LoadBase(nullShader, false);
}

std::shared_ptr<GameFramework::IAssetData> ShadersCache::LoadBase(
  const GameFramework::Asset * asset, bool async)
{
  if (!asset)
    return m_nullShader;

  auto [it, inserted] = m_shaders.insert({asset->GetUUID(), nullptr});

  if (inserted)
  {
    it->second = std::make_shared<ShaderFile>();
    auto uploadShader = [path = asset->GetPath(), ptr = it->second]
    {
      if (auto stream = GameFramework::GetFileManager().OpenReadText(path))
        ShaderFile::ReadText(*stream, *ptr);
    };
    if (!async)
    {
      uploadShader();
    }
    else
    {
      GameFramework::GetAsyncWorker().ScheduleTask(std::move(uploadShader));
    }
  }

  return it->second->IsReadyToUse() ? it->second : m_nullShader;
}

std::shared_ptr<GameFramework::IAssetData> ShadersCache::GetBase(
  const GameFramework::Asset * asset) const noexcept
{
  if (!asset)
    return m_nullShader;

  auto it = m_shaders.find(asset->GetUUID());
  if (it == m_shaders.end() || !it->second)
    return m_nullShader;

  return it->second->IsReadyToUse() ? it->second : m_nullShader;
}

} // namespace RenderPlugin
