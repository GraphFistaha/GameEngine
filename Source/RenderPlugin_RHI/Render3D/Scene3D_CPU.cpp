#include "Scene3D_CPU.hpp"

#include <Assets/AssetCache.hpp>
#include <Render3D/Renderer/CubeRenderer.hpp>
#include <Render3D/Scene3D_GPU.hpp>
#include <Resources/MaterialCache.hpp>
#include <Resources/ShadersCache.hpp>
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
      m_boundScene->AddPipeline<CubeRenderer, Dim3D::Cube>(pipeline);
  }
}

void Scene3D_CPU::AddCube(const GameFramework::Render::Cube & cube)
{
  PushObjectWithMaterial<Dim3D::Cube>(cube, m_cubesBatches);
}

void Scene3D_CPU::SetCamera(const GameFramework::Render::Camera & camera)
{
  m_camera = camera;
}

} // namespace RenderPlugin
