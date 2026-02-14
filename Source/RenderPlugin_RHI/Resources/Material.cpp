#include "Material.hpp"

#include <Constants.hpp>

namespace RenderPlugin
{
Material::Material(std::nullptr_t)
{
  if (auto reader =
        GameFramework::GetFileManager().OpenReadBinary(g_shadersDirectory / "NullShader_frag.spv"))
  {
    reader->ReadValue(m_fragmentShader);
  }
}

size_t Material::ReadText(GameFramework::ITextFileReader & stream, Material & material)
{
  material.m_path = stream.FullPath();
  std::wstring shaderPath;
  size_t result = stream.ReadLine(shaderPath);
  auto shaderStream = GameFramework::GetFileManager().OpenReadBinary(shaderPath);
  shaderStream->ReadValue(material.m_fragmentShader);
  return result;
}

void Material::WriteText(GameFramework::ITextFileWriter & stream, const Material & material)
{
  stream.WriteLine(material.m_fragmentShader.GetPath().wstring());
}

} // namespace RenderPlugin
