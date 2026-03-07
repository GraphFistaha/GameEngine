#include "Scene2D_CPU.hpp"

#include <Render2D/Scene2D_GPU.hpp>
#include <Utility/Hash.hpp>

namespace RenderPlugin
{

Scene2D_CPU::Scene2D_CPU(Scene2D_GPU & scene)
  : m_boundScene(&scene)
{
}

Scene2D_CPU::~Scene2D_CPU()
{
  assert(m_boundScene);
  m_boundScene->SetBackground(m_background);
  for (auto && pipeline : m_rects)
    m_boundScene->AddPipeline<Rect2DRenderer, Dim2D::Rect>(pipeline);
}

void Scene2D_CPU::SetBackground(const GameFramework::Render::Color3f & color)
{
  m_background = color;
}

void Scene2D_CPU::AddRect(const GameFramework::Render::Rect2d & rect)
{
  PushObjectWithMaterial<Dim2D::Rect>(rect, m_rects);
}
} // namespace RenderPlugin
