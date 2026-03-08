#pragma once
#include <Common/RenderBatch.hpp>
#include <Common/Renderer.hpp>
#include <GameFramework.hpp>
#include <Render3D/Primitives.hpp>
#include <Resources/Material.hpp>
#include <RHI.hpp>
#include <Utility/OwnedBy.hpp>

namespace RenderPlugin
{
struct Scene3D_GPU;

class CubeRenderer : public BaseRenderer<Dim3D::Cube>,
                     public GameFramework::OwnedBy<Scene3D_GPU>
{
public:
  explicit CubeRenderer(Scene3D_GPU & scene, const PipelineSettings & settings);
  virtual ~CubeRenderer() override;
  MAKE_ALIAS_FOR_GET_OWNER(Scene3D_GPU, GetScene);

public:
  virtual void Submit() override;

protected:
  virtual bool SetBatchImpl(const Dim3D::CubeBatch & batch) override;

private:
  size_t m_hash = 0;
  std::vector<GameFramework::Mat4f> m_matricesCpuBuffer;
  RHI::IBufferUniformDescriptor * m_vpDescriptor;
  RHI::ISubpass * m_renderPass = nullptr;
  RHI::IBufferGPU * m_matricesBuffer = nullptr;
};
} // namespace RenderPlugin
