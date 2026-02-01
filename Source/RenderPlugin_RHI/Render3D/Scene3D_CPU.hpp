#pragma once
#include <GameFramework.hpp>

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
  std::vector<GameFramework::Render::Cube> m_cubesToDraw;
  size_t m_cubesHash = 0;
  GameFramework::Render::Camera m_camera;
};
} // namespace RenderPlugin
