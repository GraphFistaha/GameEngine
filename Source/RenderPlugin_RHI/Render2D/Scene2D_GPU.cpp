#include "Scene2D_GPU.hpp"

#include <Constants.hpp>
#include <Files/FileManager.hpp>
#include <Resources/ShaderFile.hpp>

namespace RenderPlugin
{

Scene2D_GPU::Scene2D_GPU(InternalDevice & device)
  : OwnedBy<InternalDevice>(device)
{
}

Scene2D_GPU::~Scene2D_GPU() = default;

void Scene2D_GPU::SetBackground(const GameFramework::Render::Color3f & color)
{
  //m_backgroundRenderer.SetBackground(color);
}

void Scene2D_GPU::Invalidate()
{
  //TODO: m_renderPass->SetDirtyCommands();
}

bool Scene2D_GPU::ShouldBeInvalidated() const noexcept
{
  return false;
}

} // namespace RenderPlugin
