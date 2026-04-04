#include "Material.hpp"

#include <Assets/AssetsRegistry.hpp>
#include <Constants.hpp>
#include <Resources/ShadersCache.hpp>
#include <Resources/TextureCache.hpp>

namespace RenderPlugin
{
Material::Material(std::nullptr_t)
{
}

size_t Material::ReadText(GameFramework::ITextFileReader & stream, Material & material)
{
  material.m_path = stream.FullPath();
  std::wstring shaderPath;
  std::wstring diffuseTexture;
  size_t result = stream.ReadLine(shaderPath);
  size_t result2 = stream.ReadLine(diffuseTexture);
  auto * shadersCache = GameFramework::GetAssetCacheRegistry().Get<ShadersCache>();
  auto * textureCache = GameFramework::GetAssetCacheRegistry().Get<TextureCache>();
  auto * shaderAsset = GameFramework::GetAssetsRegistry().GetAsset(shaderPath);
  material.m_shader = shadersCache->Load<ShaderFile>(shaderAsset, false /*async*/);

  auto * diffTextureAsset = GameFramework::GetAssetsRegistry().GetAsset(diffuseTexture);
  material.m_diffuseTexture = textureCache->Load<Texture2D>(diffTextureAsset, false /*async*/);
  return result + result2;
}

void Material::WriteText(GameFramework::ITextFileWriter & stream, const Material & material)
{
  stream.WriteLine(material.m_shader->GetPath().wstring());
}

bool Material::IsReadyToUse() const noexcept
{
  return !m_path.empty() && m_shader && m_shader->IsReadyToUse() && m_diffuseTexture &&
         m_diffuseTexture->IsReadyToUse();
}

} // namespace RenderPlugin
