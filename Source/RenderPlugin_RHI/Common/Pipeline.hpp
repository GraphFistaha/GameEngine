#pragma once
#include <unordered_map>

#include <Assets/AssetCache.hpp>
#include <Assets/AssetsRegistry.hpp>
#include <Common/PipelineSettings.hpp>
#include <Common/RenderBatch.hpp>
#include <Resources/MaterialCache.hpp>

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
  auto * materialCache = GameFramework::GetAssetCacheRegistry().Get<MaterialCache>();
  assert(materialCache);
  auto && materialPtr = materialCache->Load<Material>(obj.GetMaterial());
  if (materialPtr && materialPtr->IsReadyToUse())
  {
    RenderPlugin::PipelineSettings settings(*materialPtr->GetFragmentShader());
    // sorts all cubes on groups with same pipeline settings
    auto [it, inserted] = container.insert({settings, RenderPlugin::RenderBatch<PrimT>{}});
    it->second.Push(obj);
  }
}

} // namespace RenderPlugin
