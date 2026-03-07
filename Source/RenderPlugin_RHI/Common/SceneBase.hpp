#pragma once
#include <functional>
#include <unordered_map>
#include <vector>

#include <Common/Pipeline.hpp>
#include <Common/PipelineSettings.hpp>
#include <Common/Renderer.hpp>
#include <Utility/Utility.hpp>

namespace RenderPlugin
{

template<typename Derived>
struct SceneBase
{
  virtual ~SceneBase() = default;

  template<typename RendererT, typename RenderPrimT>
  bool AddPipeline(const Pipeline<RenderPrimT> & pipeline)
  {
    bool res = true;
    auto it = m_renderers.find(pipeline.first);
    if (it != m_renderers.end())
    {
      it->second->SetBatch(pipeline.second);
      it->second->Submit();
    }
    else
    {
      if (auto * derived = GameFramework::Utils::FastDynamicCast<Derived>(this))
        m_tasksToCreateRenderers.emplace_back(
          [settings = pipeline.first, derived]
          { return std::make_pair(settings, std::make_unique<RendererT>(*derived, settings)); });
    }
    return res;
  }

  void UpdateRenderers()
  {
    for (auto && task : m_tasksToCreateRenderers)
    {
      m_renderers.insert(task());
    }
    m_tasksToCreateRenderers.clear();
  }

private:
  using RendererConstructor = std::function<std::pair<PipelineSettings, RendererUPtr>()>;
  std::vector<RendererConstructor> m_tasksToCreateRenderers;
  std::unordered_map<PipelineSettings, RendererUPtr> m_renderers;
};

} // namespace RenderPlugin
