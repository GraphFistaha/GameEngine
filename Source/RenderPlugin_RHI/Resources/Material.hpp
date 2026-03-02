#pragma once
#include <Assets/Asset.hpp>
#include <Files/FileManager.hpp>
#include <Resources/ShaderFile.hpp>

namespace RenderPlugin
{

struct Material : public GameFramework::IAssetData
{
  Material() = default;
  explicit Material(std::nullptr_t);

public:
  std::filesystem::path GetPath() const noexcept { return m_path; }
  const std::filesystem::path & GetFragmentShader() const & noexcept { return m_shaderPath; }

public:
  static size_t ReadText(GameFramework::ITextFileReader & stream, Material & material);
  static void WriteText(GameFramework::ITextFileWriter & stream, const Material & material);

public:
  virtual bool IsReadyToUse() const noexcept override;

private:
  std::filesystem::path m_path;
  std::filesystem::path m_shaderPath;
};

} // namespace RenderPlugin
