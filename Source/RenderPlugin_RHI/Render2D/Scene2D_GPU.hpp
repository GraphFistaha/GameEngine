#pragma once

#include <Common/SceneBase.hpp>
#include <Devices/InternalDeviceInterface.hpp>
#include <GameFramework.hpp>
#include <Render2D/Renderer/BackgroundRenderer.hpp>
#include <Utility/OwnedBy.hpp>

namespace RenderPlugin
{

struct Scene2D_GPU final : public SceneBase<Scene2D_GPU>,
                           public GameFramework::OwnedBy<InternalDevice>
{
  explicit Scene2D_GPU(InternalDevice & device);
  virtual ~Scene2D_GPU() override;
  MAKE_ALIAS_FOR_GET_OWNER(InternalDevice, GetDevice);

  void SetBackground(const GameFramework::Render::Color3f & color);

public:
  void Invalidate();
  bool ShouldBeInvalidated() const noexcept;

private:
  BackgroundRenderer m_backgroundRenderer;
};

} // namespace RenderPlugin
