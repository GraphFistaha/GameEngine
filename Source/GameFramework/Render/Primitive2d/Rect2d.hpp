#pragma once

#include <Assets/Asset.hpp>
#include <Render/RenderPrimitive.hpp>

namespace GameFramework::Render
{
struct GAME_FRAMEWORK_API Rect2d : public IRenderPrimitive
{
  Rect2d() = default;
  Rect2d(float left, float top, float width, float height);

public: // Hashable
  size_t Hash() const noexcept;

public:
  float X() const noexcept { return m_left; }
  float Y() const noexcept { return m_top; }
  float Width() const noexcept { return m_width; }
  float Height() const noexcept { return m_height; }
  Asset * GetMaterial() const noexcept { return m_material; }

private:
  float m_left = 0;
  float m_top = 0;
  float m_width = 0;
  float m_height = 0;
  Asset * m_material = nullptr;
};

/// concepts
static_assert(Hashable<Rect2d>);

} // namespace GameFramework::Render
