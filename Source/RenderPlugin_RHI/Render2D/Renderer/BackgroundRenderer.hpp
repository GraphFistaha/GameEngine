#pragma once
#include <GameFramework.hpp>
#include <Utility/OwnedBy.hpp>
#include <RHI.hpp>

namespace RenderPlugin
{
struct Scene2D_GPU;

class BackgroundRenderer : public GameFramework::OwnedBy<Scene2D_GPU>
{
public:
  explicit BackgroundRenderer(Scene2D_GPU & scene);
  ~BackgroundRenderer();
  MAKE_ALIAS_FOR_GET_OWNER(Scene2D_GPU, GetScene);

public:
  void SetBackground(const GameFramework::Color3f & color);
  void Submit();

private:
  RHI::ISubpass * m_renderPass = nullptr;
  RHI::IBufferGPU * m_colorBuffer = nullptr;
  RHI::IBufferUniformDescriptor * m_colorDescriptor = nullptr;
};
} // namespace RenderPlugin
