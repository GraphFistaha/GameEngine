#pragma once

#include <type_traits>
#include <unordered_set>

#include <Common/SceneBase.hpp>
#include <Devices/InternalDeviceInterface.hpp>
#include <GameFramework.hpp>
#include <Render3D/Primitives.hpp>
#include <RHI.hpp>
#include <Utility/OwnedBy.hpp>

namespace RenderPlugin
{

struct ViewProjection final
{
  GameFramework::Mat4f view;
  GameFramework::Mat4f projection;
};

struct Scene3D_GPU final : public SceneBase<Scene3D_GPU>,
                           public GameFramework::OwnedBy<InternalDevice>
{
  explicit Scene3D_GPU(InternalDevice & device);
  virtual ~Scene3D_GPU() override;
  MAKE_ALIAS_FOR_GET_OWNER(InternalDevice, GetDevice);

  void SetCamera(const GameFramework::Render::Camera & camera);

public:
  RHI::IBufferGPU * GetViewProjectionBuffer();

public:
  void Invalidate();
  bool ShouldBeInvalidated() const noexcept;

private:
  RHI::IBufferGPU * m_viewProjBuffer = nullptr;
  // you should delete renderers which is not used during frame
};

} // namespace RenderPlugin

/*
Каждый рендерер привязан к одному материалу
При присвоении кубиков должны распределить все кубики по рендерерам в зависимости от материала
палитра материалов храниться здесь в Scene3D_GPU
материал настривает пайплайн - загружает и привязывает фрагментный шейдер, выделяет буфер сэмплеров

*/
