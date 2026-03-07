#pragma once
#include <Common/Pipeline.hpp>
#include <GameFramework.hpp>
#include <Render3D/Primitives.hpp>

namespace RenderPlugin
{
struct Scene3D_GPU;

struct Scene3D_CPU final : public GameFramework::Render::IRenderableScene3D
{
  explicit Scene3D_CPU(Scene3D_GPU & scene);
  virtual ~Scene3D_CPU() override;

  virtual void AddCube(const GameFramework::Render::Cube & cube) override;
  virtual void SetCamera(const GameFramework::Render::Camera & camera) override;

private:
  Scene3D_GPU * m_boundScene = nullptr;
  PipelinesContainer<Dim3D::Cube> m_cubesBatches;
  GameFramework::Render::Camera m_camera;
};
} // namespace RenderPlugin
