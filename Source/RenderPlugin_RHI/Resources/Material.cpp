#include "Material.hpp"

#include <Constants.hpp>

namespace RenderPlugin
{
Material::Material(std::nullptr_t)
{
  m_shaderPath = g_shadersDirectory / "NullShader_frag.spv";
}

size_t Material::ReadText(GameFramework::ITextFileReader & stream, Material & material)
{
  material.m_path = stream.FullPath();
  std::wstring shaderPath;
  size_t result = stream.ReadLine(shaderPath);
  material.m_shaderPath = shaderPath;
  return result;
}

void Material::WriteText(GameFramework::ITextFileWriter & stream, const Material & material)
{
  stream.WriteLine(material.m_shaderPath.wstring());
}

bool Material::IsReadyToUse() const noexcept
{
  return !m_path.empty() && !m_shaderPath.empty();
}

} // namespace RenderPlugin
