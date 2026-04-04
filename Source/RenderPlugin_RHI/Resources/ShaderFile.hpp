#pragma once
#include <vector>

#include <Assets/Asset.hpp>
#include <Files/FileStream.hpp>

namespace RenderPlugin
{
static const std::filesystem::path g_shadersDirectory(SHADERS_DIRECTORY);
/// precompiled shader spir-v file
class ShaderFile final : public GameFramework::IAssetData
{
public:
  ShaderFile() = default;
  std::filesystem::path GetPath() const noexcept { return m_path; }
  const std::vector<uint32_t> & GetSpirV() const & noexcept { return m_data; }

  virtual bool IsReadyToUse() const noexcept override;

public:
  static size_t ReadBinary(GameFramework::IBinaryFileReader & stream, ShaderFile & file);

public:
  static size_t ReadText(GameFramework::ITextFileReader & stream, ShaderFile & result);

public: //Hashable
  size_t Hash() const noexcept;
  bool operator==(const ShaderFile & rhs) const noexcept;

private:
  std::filesystem::path m_path = g_shadersDirectory / "NullShader_frag.spv";
  std::vector<uint32_t> m_data;
};

} // namespace RenderPlugin
