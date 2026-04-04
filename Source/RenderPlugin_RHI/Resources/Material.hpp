#pragma once
#include <Assets/Asset.hpp>
#include <Files/FileManager.hpp>
#include <Resources/ShaderFile.hpp>
#include <Resources/Texture2D.hpp>

namespace RenderPlugin
{

struct Material : public GameFramework::IAssetData
{
  Material() = default;
  explicit Material(std::nullptr_t);

public:
  std::filesystem::path GetPath() const noexcept { return m_path; }
  std::shared_ptr<ShaderFile> GetFragmentShader() const { return m_shader; }
  std::shared_ptr<Texture2D> GetDiffuseTexture() const { return m_diffuseTexture; }

public:
  static size_t ReadText(GameFramework::ITextFileReader & stream, Material & material);
  static void WriteText(GameFramework::ITextFileWriter & stream, const Material & material);

public:
  virtual bool IsReadyToUse() const noexcept override;

private:
  std::filesystem::path m_path;
  std::shared_ptr<ShaderFile> m_shader;
  std::shared_ptr<Texture2D> m_diffuseTexture;
};

} // namespace RenderPlugin
