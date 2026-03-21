#include "Texture2D.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RenderPlugin
{
bool TextureFormat::operator==(TextureFormat rhs) const noexcept
{
  return width == rhs.width && height == rhs.height && channels == rhs.channels;
}

size_t TextureFormat::Hash() const noexcept
{
  return static_cast<size_t>(*reinterpret_cast<const uint32_t *>(this));
}

RHI::HostImageFormat TextureFormat::GetHostImageFormat() const noexcept
{
  switch (channels)
  {
    case 1:
      return RHI::HostImageFormat::R8;
    case 2:
      return RHI::HostImageFormat::RG8;
    case 3:
      return RHI::HostImageFormat::RGB8;
    case 4:
      return RHI::HostImageFormat::RGBA8;
    default:
      return RHI::HostImageFormat::R8;
  }
}

RHI::ImageFormat TextureFormat::GetInternalImageFormat() const noexcept
{
  switch (channels)
  {
    case 1:
      return RHI::ImageFormat::R8;
    case 2:
      return RHI::ImageFormat::RG8;
    case 3:
      return RHI::ImageFormat::RGB8;
    case 4:
      return RHI::ImageFormat::RGBA8;
    default:
      return RHI::ImageFormat::R8;
  }
}

RHI::TextureExtent TextureFormat::GetExtent() const noexcept
{
  return {width, height, 1};
}

} // namespace RenderPlugin

namespace RenderPlugin
{
void Texture2D::BindToGpuContainer(RHI::ITexture * texture, uint32_t layer) noexcept
{
  m_texture = texture;
  m_layer = layer;
}

bool Texture2D::IsReadyToUse() const noexcept
{
  return false;
}

size_t Texture2D::ReadBinary(GameFramework::IBinaryFileReader & stream, Texture2D & texture)
{
  stbi_io_callbacks callbacks{};
  callbacks.read = [](void * user, char * data, int size) -> int
  {
    auto * stream = reinterpret_cast<GameFramework::IBinaryFileReader *>(user);
    std::span<std::byte> bytes(reinterpret_cast<std::byte *>(data), size);
    return static_cast<int>(stream->Read(bytes));
  };
  callbacks.skip = [](void * user, int n)
  {
    auto * stream = reinterpret_cast<GameFramework::IBinaryFileReader *>(user);
    stream->Seek(n, GameFramework::SeekDirection::Current);
  };
  callbacks.eof = [](void * user) -> int
  {
    auto * stream = reinterpret_cast<GameFramework::IBinaryFileReader *>(user);
    return stream->Eof();
  };

  int width, height, channels;
  if (!texture.m_texture)
  {
    stbi_info_from_callbacks(&callbacks, &stream, &width, &height, &channels);
    stream.Seek(0, GameFramework::SeekDirection::Begin);
    texture.m_format.width = static_cast<uint16_t>(width);
    texture.m_format.height = static_cast<uint16_t>(height);
    texture.m_format.channels = static_cast<uint16_t>(channels);
  }
  else
  {
    stbi_uc * imgData = stbi_load_from_callbacks(&callbacks, &stream, &width, &height, &channels,
                                                 texture.GetFormat().channels);
    RHI::TextureExtent extent{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};
    RHI::UploadImageArgs args{};
    args.srcTexture.pixelData = imgData;
    args.srcTexture.extent = extent;
    args.srcTexture.layersCount = 1;
    args.srcTexture.format = texture.GetFormat().GetHostImageFormat();
    args.layersCount = 1;
    args.layerIndex = texture.m_layer;
    args.copyRegion = {{0, 0, 0}, extent};
    args.dstOffset = {0, 0, 0};
    texture.m_texture->UploadImage(args);
    stbi_image_free(imgData);
  }

  return 1;
}

} // namespace RenderPlugin
