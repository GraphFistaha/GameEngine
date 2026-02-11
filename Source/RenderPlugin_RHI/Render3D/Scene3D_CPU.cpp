#include "Scene3D_CPU.hpp"

#include <Render3D/Scene3D_GPU.hpp>
#include <Resources/MaterialManager.hpp>
#include <Utility/Hash.hpp>
#include <Utility/Uuid.hpp>

namespace RenderPlugin
{
Scene3D_CPU::Scene3D_CPU(Scene3D_GPU & scene)
  : m_boundScene(&scene)
{
}
Scene3D_CPU::~Scene3D_CPU()
{
  if (m_boundScene)
  {
    m_boundScene->SetCamera(m_camera);
    for (auto && pipeline : m_cubesBatches)
      m_boundScene->AddPipeline(pipeline);

  }
}

void Scene3D_CPU::AddCube(const GameFramework::Render::Cube & cube)
{
  // sorts all cubes on groups with same pipeline settings
  auto * material = GetMaterialManager().LoadMaterial(cube.GetMaterial());
  assert(material != nullptr);
  PipelineSettings settings(material->GetFragmentShader());
  auto [it, inserted] = m_cubesBatches.insert({settings, Dim3D::CubeBatch{}});
  it->second.Push(cube);
}

void Scene3D_CPU::SetCamera(const GameFramework::Render::Camera & camera)
{
  m_camera = camera;
}

} // namespace RenderPlugin
