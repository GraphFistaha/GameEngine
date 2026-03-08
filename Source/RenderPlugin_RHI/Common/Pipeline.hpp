#pragma once
#include <unordered_map>

#include <Assets/AssetCache.hpp>
#include <Assets/AssetsRegistry.hpp>
#include <Common/PipelineSettings.hpp>
#include <Common/RenderBatch.hpp>
#include <Resources/MaterialCache.hpp>
#include <Resources/ShadersCache.hpp>

namespace RenderPlugin
{

template<typename PrimT>
using Pipeline = std::pair<PipelineSettings, RenderBatch<PrimT>>;

template<typename PrimT>
using PipelinesContainer = std::unordered_map<PipelineSettings, RenderBatch<PrimT>>;


template<typename PrimT>
inline void PushObjectWithMaterial(const PrimT & obj,
                                   RenderPlugin::PipelinesContainer<PrimT> & container)
{
  auto * cache = GameFramework::GetAssetCacheRegistry().Get<MaterialCache>();
  auto * shadersCache = GameFramework::GetAssetCacheRegistry().Get<ShadersCache>();
  auto && materialPtr = cache->Load<Material>(obj.GetMaterial());
  if (materialPtr)
  {
    auto * asset = GameFramework::GetAssetsRegistry().GetAsset(materialPtr->GetFragmentShader());
    auto && shaderPtr = shadersCache->Load<ShaderFile>(asset, false);
    if (shaderPtr)
    {
      RenderPlugin::PipelineSettings settings(*shaderPtr);
      // sorts all cubes on groups with same pipeline settings
      auto [it, inserted] = container.insert({settings, RenderPlugin::RenderBatch<PrimT>{}});
      it->second.Push(obj);
    }
  }
}

} // namespace RenderPlugin
