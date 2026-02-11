#include "Material.hpp"

#include <Constants.hpp>

namespace RenderPlugin
{
Material::Material(std::nullptr_t)
{
  if (auto reader =
        GameFramework::GetFileManager().OpenRead(g_shadersDirectory / "NullShader_frag.spv"))
  {
    reader->ReadValue(m_fragmentShader);
  }
}

size_t Material::ReadBinary(GameFramework::IFileReader & stream, Material & material)
{
  material.m_path = stream.FullPath();
  std::wstring shaderPath;
  size_t result = stream.ReadValue(shaderPath);
  auto shaderStream = GameFramework::GetFileManager().OpenRead(shaderPath);
  shaderStream->ReadValue(material.m_fragmentShader);
  return result;
}

void Material::WriteBinary(GameFramework::IFileWriter & stream, const Material & material)
{
  stream.WriteValue(material.m_fragmentShader.GetPath().wstring());
}

} // namespace RenderPlugin
