#pragma once
#include <GameFramework.hpp>
#include <RHI.hpp>
#include <Utility/OwnedBy.hpp>

namespace RenderPlugin
{
struct Scene2D_GPU;

class Rect2DRenderer : public GameFramework::OwnedBy<Scene2D_GPU>
{
public:
  explicit Rect2DRenderer(Scene2D_GPU & scene);
  ~Rect2DRenderer();
  MAKE_ALIAS_FOR_GET_OWNER(Scene2D_GPU, GetScene);

public:
  void TrySetRects(size_t newHash, std::span<const GameFramework::Render::Rect2d> rects);
  void Submit();

private:
  size_t m_hash = 0;
  std::vector<std::pair<float, float>> m_verticesCpuBuffer;
  RHI::ISubpass * m_renderPass = nullptr;
  RHI::IBufferGPU * m_verticesBuffer = nullptr;
};
} // namespace RenderPlugin
