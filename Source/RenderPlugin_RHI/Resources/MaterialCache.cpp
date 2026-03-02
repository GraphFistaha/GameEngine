#include "MaterialCache.hpp"

#include <mutex>
#include <queue>
#include <unordered_map>

#include <Assets/AssetsRegistry.hpp>
#include <Game/Async.hpp>
#include <Resources/Material.hpp>

namespace RenderPlugin
{

MaterialCache::MaterialCache()
{
  auto * nullMaterial = GameFramework::GetAssetsRegistry().GetAsset("Materials/NullMaterial.mat");
  m_nullMaterial = LoadBase(nullMaterial, false);
}


std::shared_ptr<GameFramework::IAssetData> MaterialCache::LoadBase(
  const GameFramework::Asset * asset, bool async)
{
  if (!asset)
    return m_nullMaterial;

  auto [it, inserted] = m_materials.insert({asset->GetUUID(), nullptr});

  if (inserted)
  {
    it->second = std::make_shared<Material>();
    auto uploadMaterial = [path = asset->GetPath(), matPtr = it->second]
    {
      if (auto stream = GameFramework::GetFileManager().OpenReadText(path))
        Material::ReadText(*stream, *matPtr);
    };
    if (!async)
    {
      uploadMaterial();
    }
    else
    {
      GameFramework::GetAsyncWorker().ScheduleTask(std::move(uploadMaterial));
    }
  }

  return it->second->IsReadyToUse() ? it->second : m_nullMaterial;
}

std::shared_ptr<GameFramework::IAssetData> MaterialCache::GetBase(
  const GameFramework::Asset * asset) const noexcept
{
  if (!asset)
    return m_nullMaterial;

  auto it = m_materials.find(asset->GetUUID());
  if (it == m_materials.end() || !it->second)
    it = m_materials.find(GameFramework::Uuid());

  return it->second->IsReadyToUse() ? it->second : m_nullMaterial;
}
} // namespace RenderPlugin
