#include "CubeRenderer.hpp"

#include <algorithm>
#include <ranges>

#include <Constants.hpp>
#include <GameFramework.hpp>
#include <Render3D/Scene3D_GPU.hpp>
#include <Resources/ShadersCache.hpp>
#include <Resources/TextureCache.hpp>

namespace RenderPlugin
{

CubeRenderer::CubeRenderer(Scene3D_GPU & scene, const PipelineSettings & settings)
  : OwnedBy<Scene3D_GPU>(scene)
  , m_renderPass(scene.GetDevice().GetFramebuffer().CreateSubpass())
{
  m_matricesCpuBuffer.reserve(128);
  // должно заполняться ScreenDevice
  auto && subpassConfig = m_renderPass->GetConfiguration();
  scene.GetDevice().ConfigurePipeline(subpassConfig);
  subpassConfig.EnableDepthTest(true);
  subpassConfig.SetMeshTopology(RHI::MeshTopology::Triangle);
  subpassConfig.AddInputBinding(0, sizeof(GameFramework::Mat4f),
                                RHI::InputBindingType::InstanceData);
  subpassConfig.AddInputAttribute(0, 0, 0, 4, RHI::InputAttributeElementType::FLOAT);
  subpassConfig.AddInputAttribute(0, 1, 1 * sizeof(GameFramework::Vec4f), 4,
                                  RHI::InputAttributeElementType::FLOAT);
  subpassConfig.AddInputAttribute(0, 2, 2 * sizeof(GameFramework::Vec4f), 4,
                                  RHI::InputAttributeElementType::FLOAT);
  subpassConfig.AddInputAttribute(0, 3, 3 * sizeof(GameFramework::Vec4f), 4,
                                  RHI::InputAttributeElementType::FLOAT);
  {
    std::shared_ptr<ShaderFile> vertShader;
    if (auto * asset = GameFramework::GetAssetsRegistry().GetAsset("Shaders/3D/Cube.vert"))
      if (auto * cache = GameFramework::GetAssetCacheRegistry().Get<ShadersCache>())
        vertShader = cache->Load<ShaderFile>(asset, false /*async*/);
    if (vertShader)
      subpassConfig.AttachShader(RHI::ShaderType::Vertex, vertShader->GetSpirV());
  }
  subpassConfig.AttachShader(RHI::ShaderType::Fragment, settings.GetShader().GetSpirV());
  m_vpDescriptor = subpassConfig.DeclareUniform({0, 0}, RHI::ShaderType::Vertex);
  m_vpDescriptor->AssignBuffer(*scene.GetViewProjectionBuffer());
  if (auto * cache = GameFramework::GetAssetCacheRegistry().Get<TextureCache>())
  {
    m_samplers = cache->BindToPipeline(&subpassConfig, {0, 1}, RHI::ShaderType::Fragment);
    m_samplersHash = cache->PaletteHash();
  }
}

CubeRenderer::~CubeRenderer()
{
  //TODO: remove subpass
  GetScene().GetDevice().GetContext().DeleteBuffer(m_matricesBuffer);
}

bool CubeRenderer::SetBatchImpl(const Dim3D::CubeBatch & batch)
{
  if (batch.ObjectsHash() != m_hash)
  {
    size_t oldCapacity = m_matricesCpuBuffer.capacity();
    m_matricesCpuBuffer.clear();
    for (auto && cube : batch.GetObjects())
      m_matricesCpuBuffer.push_back(cube.GetTransform());

    size_t newCapacity = m_matricesCpuBuffer.capacity();

    if (newCapacity != oldCapacity || !m_matricesBuffer)
    {
      RHI::IBufferGPU * newVerticesBuffer =
        GetScene().GetDevice().GetContext().CreateBuffer(newCapacity * sizeof(GameFramework::Mat4f),
                                                         RHI::BufferGPUUsage::VertexBuffer, false);
      //TODO: Delete old verticesBuffer
      m_matricesBuffer = newVerticesBuffer;
    }
    m_matricesBuffer->UploadAsync(m_matricesCpuBuffer.data(),
                                  m_matricesCpuBuffer.size() * sizeof(GameFramework::Mat4f));
    m_hash = batch.ObjectsHash();
  }
  return true;
}

void CubeRenderer::Submit()
{
  auto * cache = GameFramework::GetAssetCacheRegistry().Get<TextureCache>();
  if (m_renderPass && m_renderPass->ShouldBeInvalidated() && !m_matricesCpuBuffer.empty())
  {
    // update descriptors
    if (cache && m_samplersHash != cache->PaletteHash())
    {
      cache->UpdatePipeline(m_samplers);
      m_samplersHash = cache->PaletteHash();
    }

    auto extent = GetScene().GetDevice().GetFramebuffer().GetExtent();
    if (m_renderPass->BeginPass())
    {
      m_renderPass->SetScissor(0, 0, extent[0], extent[1]);
      m_renderPass->SetViewport(static_cast<float>(extent[0]), static_cast<float>(extent[1]));
      m_renderPass->BindVertexBuffer(0, *m_matricesBuffer);
      m_renderPass->DrawVertices(36, m_matricesCpuBuffer.size());
      m_renderPass->EndPass();
    }
  }
}

} // namespace RenderPlugin
