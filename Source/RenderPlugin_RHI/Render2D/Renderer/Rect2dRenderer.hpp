#pragma once
#include <Common/RenderBatch.hpp>
#include <Common/Renderer.hpp>
#include <GameFramework.hpp>
#include <Render2D/Primitives.hpp>
#include <RHI.hpp>
#include <Utility/OwnedBy.hpp>

namespace RenderPlugin
{
struct Scene2D_GPU;

class Rect2DRenderer : public BaseRenderer<Dim2D::Rect>,
                       public GameFramework::OwnedBy<Scene2D_GPU>
{
public:
  explicit Rect2DRenderer(Scene2D_GPU & scene, const PipelineSettings & settings);
  virtual ~Rect2DRenderer() override;
  MAKE_ALIAS_FOR_GET_OWNER(Scene2D_GPU, GetScene);

public:
  virtual void Submit() override;

protected:
  virtual bool SetBatchImpl(const Dim2D::RectBatch & batch) override;

private:
  size_t m_hash = 0;
  std::vector<std::pair<float, float>> m_verticesCpuBuffer;
  RHI::ISubpass * m_renderPass = nullptr;
  RHI::IBufferGPU * m_verticesBuffer = nullptr;
};
} // namespace RenderPlugin
