#include "Scene3D_CPU.hpp"

#include <Render3D/Scene3D_GPU.hpp>
#include <Utility/Hash.hpp>

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
    m_boundScene->TrySetCubes(m_cubesHash, m_cubesToDraw);
    m_boundScene->Draw();
  }
}

void Scene3D_CPU::AddCube(const GameFramework::Render::Cube & cube)
{
  m_cubesToDraw.push_back(cube);
  GameFramework::Utils::combined_hash(m_cubesHash, cube);
}

void Scene3D_CPU::SetCamera(const GameFramework::Render::Camera & camera)
{
  m_camera = camera;
}

} // namespace RenderPlugin
