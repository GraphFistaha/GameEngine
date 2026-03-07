#include "Rect2DRenderer.hpp"

#include <Constants.hpp>
#include <GameFramework.hpp>
#include <Render2D/Scene2D_GPU.hpp>
#include <Resources/ShadersCache.hpp>

namespace RenderPlugin
{
Rect2DRenderer::Rect2DRenderer(Scene2D_GPU & scene, const PipelineSettings & settings)
  : OwnedBy<Scene2D_GPU>(scene)
  , m_renderPass(scene.GetDevice().GetFramebuffer().CreateSubpass())
{
  m_verticesCpuBuffer.reserve(128);
  auto && subpassConfig = m_renderPass->GetConfiguration();
  scene.GetDevice().ConfigurePipeline(subpassConfig);
  subpassConfig.EnableDepthTest(true);
  subpassConfig.SetMeshTopology(RHI::MeshTopology::Triangle);
  subpassConfig.AddInputBinding(0, 2 * sizeof(float), RHI::InputBindingType::VertexData);
  subpassConfig.AddInputAttribute(0, 0, 0, 2, RHI::InputAttributeElementType::FLOAT);
  {
    std::shared_ptr<ShaderFile> vertShader;
    if (auto * asset = GameFramework::GetAssetsRegistry().GetAsset("Shaders/2D/rect2d.vert"))
      if (auto * cache = GameFramework::GetAssetCacheRegistry().Get<ShadersCache>())
        vertShader = cache->Load<ShaderFile>(asset, false /*async*/);
    if (vertShader)
      subpassConfig.AttachShader(RHI::ShaderType::Vertex, vertShader->GetSpirV());
  }
  {
    subpassConfig.AttachShader(RHI::ShaderType::Fragment, settings.GetShader().GetSpirV());
  }
}

Rect2DRenderer::~Rect2DRenderer()
{
  //TODO: remove subpass
  //TODO: remove buffer
}

bool Rect2DRenderer::SetBatchImpl(const Dim2D::RectBatch & batch)
{
  if (batch.ObjectsHash() != m_hash)
  {
    size_t oldCapacity = m_verticesCpuBuffer.capacity();
    m_verticesCpuBuffer.clear();
    for (auto && rect : batch.GetObjects())
    {
      const float l = rect.X();
      const float t = rect.Y();
      const float r = rect.X() + rect.Width();
      const float b = rect.Y() + rect.Height();
      m_verticesCpuBuffer.emplace_back(l, t);
      m_verticesCpuBuffer.emplace_back(r, t);
      m_verticesCpuBuffer.emplace_back(l, b);
      m_verticesCpuBuffer.emplace_back(l, b);
      m_verticesCpuBuffer.emplace_back(r, t);
      m_verticesCpuBuffer.emplace_back(r, b);
    }

    size_t newCapacity = m_verticesCpuBuffer.capacity();

    if (newCapacity != oldCapacity || !m_verticesBuffer)
    {
      RHI::IBufferGPU * newVerticesBuffer =
        GetScene().GetDevice().GetContext().CreateBuffer(newCapacity * 6 * 2 * sizeof(float),
                                                         RHI::BufferGPUUsage::VertexBuffer, false);
      GetScene().GetDevice().GetContext().DeleteBuffer(m_verticesBuffer);
      m_verticesBuffer = newVerticesBuffer;
    }
    m_verticesBuffer->UploadAsync(m_verticesCpuBuffer.data(),
                                  m_verticesCpuBuffer.size() * 6 * 2 * sizeof(float));
    m_hash = batch.ObjectsHash();
  }
  return true;
}

void Rect2DRenderer::Submit()
{
  if (m_renderPass && m_renderPass->ShouldBeInvalidated() && !m_verticesCpuBuffer.empty())
  {
    auto extent = GetScene().GetDevice().GetFramebuffer().GetExtent();
    m_renderPass->BeginPass();
    m_renderPass->SetScissor(0, 0, extent[0], extent[1]);
    m_renderPass->SetViewport(static_cast<float>(extent[0]), static_cast<float>(extent[1]));
    m_renderPass->BindVertexBuffer(0, *m_verticesBuffer);
    m_renderPass->DrawVertices(m_verticesCpuBuffer.size(), 1);
    m_renderPass->EndPass();
  }
}

} // namespace RenderPlugin
