#pragma once
#include <Common/Pipeline.hpp>
#include <GameFramework.hpp>
#include <Render2D/Primitives.hpp>
#include <Resources/Material.hpp>

namespace RenderPlugin
{
struct Scene2D_GPU;

struct Scene2D_CPU final : public GameFramework::Render::IRenderableScene2D
{
  explicit Scene2D_CPU(Scene2D_GPU & scene);
  virtual ~Scene2D_CPU() override;

  virtual void SetBackground(const GameFramework::Render::Color3f & color) override;
  virtual void AddRect(const GameFramework::Render::Rect2d & rect) override;

private:
  Scene2D_GPU * m_boundScene = nullptr;
  GameFramework::Render::Color3f m_background;
  PipelinesContainer<Dim2D::Rect> m_rects;
};
} // namespace RenderPlugin
