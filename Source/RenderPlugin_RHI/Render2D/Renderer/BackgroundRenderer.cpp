#include "BackgroundRenderer.hpp"

#include <Assets/AssetCache.hpp>
#include <Constants.hpp>
#include <GameFramework.hpp>
#include <Render2D/Scene2D_GPU.hpp>
#include <Resources/ShadersCache.hpp>

namespace RenderPlugin
{
BackgroundRenderer::BackgroundRenderer(Scene2D_GPU & scene)
  : OwnedBy<Scene2D_GPU>(scene)
  , m_renderPass(scene.GetDevice().GetFramebuffer().CreateSubpass())
  , m_colorBuffer(scene.GetDevice().GetContext().CreateBuffer(3 * sizeof(float),
                                                              RHI::BufferGPUUsage::UniformBuffer,
                                                              true))
{
  auto && subpassConfig = m_renderPass->GetConfiguration();
  scene.GetDevice().ConfigurePipeline(subpassConfig);
  subpassConfig.EnableDepthTest(true);
  subpassConfig.SetMeshTopology(RHI::MeshTopology::TriangleFan);
  m_colorDescriptor = subpassConfig.DeclareUniform({0, 0}, RHI::ShaderType::Fragment);
  m_colorDescriptor->AssignBuffer(*m_colorBuffer);
  {
    std::shared_ptr<ShaderFile> vertShader;
    if (auto * asset = GameFramework::GetAssetsRegistry().GetAsset("Shaders/2D/background.vert"))
      if (auto * cache = GameFramework::GetAssetCacheRegistry().Get<ShadersCache>())
        vertShader = cache->Load<ShaderFile>(asset, false /*async*/);
    if (vertShader)
      subpassConfig.AttachShader(RHI::ShaderType::Vertex, vertShader->GetSpirV());
  }
  {
    std::shared_ptr<ShaderFile> fragShader;
    if (auto * asset = GameFramework::GetAssetsRegistry().GetAsset("Shaders/2D/background.frag"))
      if (auto * cache = GameFramework::GetAssetCacheRegistry().Get<ShadersCache>())
        fragShader = cache->Load<ShaderFile>(asset, false /*async*/);
    if (fragShader)
      subpassConfig.AttachShader(RHI::ShaderType::Fragment, fragShader->GetSpirV());
  }
}

BackgroundRenderer::~BackgroundRenderer()
{
  //TODO: remove subpass
  //TODO: remove buffer
}

void BackgroundRenderer::SetBackground(const GameFramework::Render::Color3f & color)
{
  m_colorBuffer->UploadSync(color.data(), color.size() * sizeof(float));
}

void BackgroundRenderer::Submit()
{
  if (m_renderPass && m_renderPass->ShouldBeInvalidated())
  {
    auto extent = GetScene().GetDevice().GetFramebuffer().GetExtent();
    m_renderPass->BeginPass();
    m_renderPass->SetScissor(0, 0, extent[0], extent[1]);
    m_renderPass->SetViewport(static_cast<float>(extent[0]), static_cast<float>(extent[1]));
    m_renderPass->DrawVertices(4, 1);
    m_renderPass->EndPass();
  }
}

} // namespace RenderPlugin
