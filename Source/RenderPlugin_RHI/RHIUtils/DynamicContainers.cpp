#include "DynamicContainers.hpp"

#include <algorithm>
#include <numeric>

namespace RHI::DynamicContainers
{


TexturePalette::TexturePalette(RHI::IContext * ctx, RHI::TextureDescription & descr)
  : m_ctx(ctx)
  , m_description(descr)
{
  assert(m_description.type == ImageType::Image2D ||
         m_description.type == RHI::ImageType::Image2D_Array);
  m_description.type = RHI::ImageType::Image2D_Array;
  m_texture = ctx->CreateTexture(m_description);
  m_freeList.insert({0, m_description.extent[2]});
}

TexturePalette::~TexturePalette()
{
  m_ctx->DeleteTexture(m_texture);
}

RHI::TexelIndex TexturePalette::Alloc(RHI::TextureExtent extent)
{
  if (extent[0] != m_description.extent[0])
    return {g_InvalidTexel, g_InvalidTexel, g_InvalidTexel};

  texel_t layersCount = extent[2];

  // find the place in freeList where we can insert this extent
  auto it = m_freeList.begin();
  while (it != m_freeList.end() && it->second < layersCount)
    it = std::next(it);

  // if place not found - extend the memory block
  if (it == m_freeList.end())
  {
    RHI::texel_t totalLayersCount = m_description.extent[2];
    m_description.extent[2] *= 4;
    auto * texture = m_ctx->CreateTexture(m_description);
    m_texture->BlitTo(texture);
    m_ctx->DeleteTexture(m_texture); //TODO: don't wait for blit is end
    m_texture = texture;
    it = m_freeList.insert(m_freeList.end(),
                           {totalLayersCount, m_description.extent[2] - totalLayersCount});
  }
  auto [newIt, node, pos] = m_allocations.insert(std::move(m_freeList.extract(it)));
  it = newIt;
  if (it->second - layersCount != 0)
    m_freeList.insert({it->first + layersCount, it->second - layersCount});
  it->second = layersCount;
  return {0, 0, it->first};
}

void TexturePalette::Delete(RHI::TexelIndex origin)
{
  if (origin[0] != 0 || origin[1] != 0)
    return;

  uint32_t layerIndex = origin[2];
  auto it = m_allocations.find(layerIndex);
  if (it == m_allocations.end())
    throw std::runtime_error("No such allocation");

  auto merge_nodes = [this](auto left, auto right)
  {
    assert(std::next(left) == right);
    if (right != m_freeList.end() && left->first + left->second == right->first)
    {
      left->second += right->second;
      m_freeList.erase(right);
    }
  };

  m_freeList.insert(std::move(m_allocations.extract(it)));
  merge_nodes(it, std::next(it));

  if (it != m_freeList.begin())
    merge_nodes(std::prev(it), it);
}

std::future<RHI::UploadResult> TexturePalette::Upload(RHI::TexelIndex origin,
                                                      RHI::HostTextureView texture)
{
  if (origin[0] != 0 || origin[1] != 0)
    return std::future<RHI::UploadResult>();

  texel_t layerIndex = origin[2];
  texel_t layersCount = texture.extent[2];
  auto it = m_allocations.find(layerIndex);
  if (it == m_allocations.end())
    throw std::runtime_error("Texture layer is not allocated");

  if (it->second < layersCount)
    throw std::runtime_error("Incorrect layers count in HostTexture");

  RHI::UploadImageArgs args{};
  args.srcTexture = texture;
  args.dstOffset = {0, 0, layerIndex};
  args.copyRegion = {{0, 0, 0}, texture.extent};
  return m_texture->UploadImage(args);
}

size_t TexturePalette::TextureHash() const noexcept
{
  return reinterpret_cast<size_t>(m_texture);
}

RHI::ITexture * TexturePalette::Handle() const noexcept
{
  return m_texture;
}

} // namespace RHI::DynamicContainers
