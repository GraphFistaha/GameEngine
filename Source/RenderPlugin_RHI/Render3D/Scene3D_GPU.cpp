#include "Scene3D_GPU.hpp"

namespace RenderPlugin
{
Scene3D_GPU::Scene3D_GPU(InternalDevice & device)
  : OwnedBy<InternalDevice>(device)
  , m_viewProjBuffer(
      device.GetContext().CreateBuffer(sizeof(ViewProjection), RHI::UniformBuffer, true))
{
}

Scene3D_GPU::~Scene3D_GPU()
{
  GetDevice().GetContext().DeleteBuffer(m_viewProjBuffer);
}

void Scene3D_GPU::SetCamera(const GameFramework::Render::Camera & camera)
{
  ViewProjection vp{camera.GetViewMatrix(), camera.GetProjectionMatrix()};
  m_viewProjBuffer->UploadSync(&vp, sizeof(vp));
}

bool Scene3D_GPU::AddPipeline(const Pipeline<Dim3D::Cube> & pipeline)
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
    m_tasksToCreateRenderers.emplace_back(
      [settings = pipeline.first, this]
      { return std::make_pair(settings, std::make_unique<CubeRenderer>(*this, settings)); });
  }
  return res;
}

RHI::IBufferGPU * Scene3D_GPU::GetViewProjectionBuffer()
{
  return m_viewProjBuffer;
}

void Scene3D_GPU::Invalidate()
{
  // for (auto&& [settings, rendererPtr] : m_renderers)
  //    rendererPtr->Invalidate();
}

bool Scene3D_GPU::ShouldBeInvalidated() const noexcept
{
  return false;
}

void Scene3D_GPU::UpdateRenderers()
{
  for (auto && task : m_tasksToCreateRenderers)
  {
    m_renderers.insert(task());
  }
  m_tasksToCreateRenderers.clear();
}
} // namespace RenderPlugin
