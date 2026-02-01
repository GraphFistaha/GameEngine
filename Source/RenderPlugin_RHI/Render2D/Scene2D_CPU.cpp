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
  m_boundScene->TrySetRects(m_rectsHash, m_rectsToDraw);
  m_boundScene->Draw();
}

void Scene2D_CPU::SetBackground(const GameFramework::Render::Color3f & color)
{
  assert(m_boundScene);
  m_boundScene->SetBackground(color);
}

void Scene2D_CPU::AddRect(const GameFramework::Render::Rect2d & rect)
{
  m_rectsToDraw.push_back(rect);
  GameFramework::Utils::combined_hash(m_rectsHash, rect);
}
} // namespace RenderPlugin
