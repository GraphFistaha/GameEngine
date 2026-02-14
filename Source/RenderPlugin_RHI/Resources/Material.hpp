#pragma once
#include <Assets/Asset.hpp>
#include <Files/FileManager.hpp>
#include <Resources/ShaderFile.hpp>

namespace RenderPlugin
{

struct Material
{
  Material() = default;
  explicit Material(std::nullptr_t);

public:
  std::filesystem::path GetPath() const noexcept { return m_path; }
  const ShaderFile & GetFragmentShader() const & noexcept { return m_fragmentShader; }

public:
  static size_t ReadText(GameFramework::ITextFileReader & stream, Material & material);
  static void WriteText(GameFramework::ITextFileWriter & stream, const Material & material);

private:
  std::filesystem::path m_path;
  ShaderFile m_fragmentShader; ///< fragment shader
};

} // namespace RenderPlugin
