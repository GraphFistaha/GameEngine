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

} // namespace RenderPlugin
