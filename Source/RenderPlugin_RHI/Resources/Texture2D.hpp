#pragma once
#include <Assets/Asset.hpp>
#include <Files/FileStream.hpp>
#include <RHI.hpp>
#include <RHIUtils/DynamicContainers.hpp>

namespace RenderPlugin
{
struct alignas(4) TextureFormat final
{
  uint16_t width : 14 = 0;
  uint16_t height : 14 = 0;
  uint16_t channels : 4 = 0; // 0 - red, 2 - rgb, 3 - rgba

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
  void BindToGpuContainer(RHI::DynamicContainers::TexturePalette * palette,
                          RHI::TexelIndex textureIndex) noexcept;
  TextureFormat GetFormat() const noexcept { return m_format; }
  RHI::TexelIndex GetIndex() const noexcept { return m_textureIndex; }

public:
  virtual bool IsReadyToUse() const noexcept override;

public:
  static size_t ReadBinary(GameFramework::IBinaryFileReader & stream, Texture2D & texture);

private:
  TextureFormat m_format;
  RHI::TexelIndex m_textureIndex;
  RHI::DynamicContainers::TexturePalette * m_palette = nullptr;
  std::future<RHI::UploadResult> m_uploadTask;
};

} // namespace RenderPlugin
