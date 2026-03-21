#pragma once
#include <Assets/Asset.hpp>
#include <Files/FileStream.hpp>
#include <RHI.hpp>

namespace RenderPlugin
{
struct alignas(4) TextureFormat final
{
  uint16_t width = 0;
  uint16_t height : 14 = 0;
  uint16_t channels : 2 = 0;

public:
  bool operator==(TextureFormat rhs) const noexcept;
  size_t Hash() const noexcept;
  RHI::HostImageFormat GetHostImageFormat() const noexcept;
  RHI::ImageFormat GetInternalImageFormat() const noexcept;
  RHI::TextureExtent GetExtent() const noexcept;
};

class Texture2D final : public GameFramework::IAssetData
{
public:
  Texture2D() = default;
  virtual ~Texture2D() override {};

public:
  void BindToGpuContainer(RHI::ITexture * texture, uint32_t layer) noexcept;
  TextureFormat GetFormat() const noexcept { return m_format; }
  uint32_t GetWidth() const noexcept { return m_format.width; }
  uint32_t GetHeight() const noexcept { return m_format.height; }

public:
  virtual bool IsReadyToUse() const noexcept override;

public:
  static size_t ReadBinary(GameFramework::IBinaryFileReader & stream, Texture2D & texture);

private:
  TextureFormat m_format;
  uint32_t m_layer = 0;
  RHI::ITexture * m_texture = nullptr;
};

} // namespace RenderPlugin
