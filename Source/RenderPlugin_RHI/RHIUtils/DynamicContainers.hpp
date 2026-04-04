#pragma once
#include <map>
#include <span>

#include <RHI.hpp>

namespace RHI
{
namespace DynamicContainers
{
struct RHI_API BufferRegion final
{
  size_t offset = 0;
  size_t size = 0;
};

//struct RHI_API DynamicGPUBuffer final
//{
//  explicit DynamicGPUBuffer(RHI::IContext & ctx, RHI::BufferGPUUsage usage);
//  ~DynamicGPUBuffer();
//
//public:
//  size_t Alloc(size_t size);
//  void Upload(size_t offset, std::span<std::byte> data, bool async);
//  void Delete(size_t offset);
//  void Reserve(size_t capacity);
//  const IBufferGPU * Handle() const noexcept;
//  bool Empty() const noexcept;
//  size_t Count() const noexcept;
//
//private:
//  IContext * m_context = nullptr;
//  RHI::BufferGPUUsage m_usage;
//  size_t m_capacity = 0;
//  IBufferGPU * m_buffer = nullptr;
//
//private:
//  DynamicGPUBuffer(const DynamicGPUBuffer & rhs) = delete;
//  DynamicGPUBuffer & operator==(const DynamicGPUBuffer & rhs) = delete;
//};


struct TexturePalette
{
  explicit TexturePalette(RHI::IContext * ctx, RHI::TextureDescription & descr);
  ~TexturePalette();

public:
  RHI::TexelIndex Alloc(RHI::TextureExtent extent);
  void Delete(RHI::TexelIndex origin);
  std::future<RHI::UploadResult> Upload(RHI::TexelIndex origin, RHI::HostTextureView texture);

  size_t TextureHash() const noexcept;
  RHI::ITexture * Handle() const noexcept;

private:
  IContext * m_ctx = nullptr;
  TextureDescription m_description;
  std::map<RHI::texel_t /*baseLayer*/, RHI::texel_t /*layersCount*/> m_freeList;
  std::map<RHI::texel_t /*baseLayer*/, RHI::texel_t /*layersCount*/> m_allocations;
  ITexture * m_texture = nullptr;
};
using TexturePaletteUPtr = std::unique_ptr<TexturePalette>;

} // namespace DynamicContainers
} // namespace RHI
