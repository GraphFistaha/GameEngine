#include "Utils.hpp"

#include <unordered_map>

namespace GameFramework::details
{

AssetType GetAssetTypeByPath(const std::filesystem::path & path) noexcept
{
  static std::unordered_map<std::string_view, AssetType> s_map{
    // ---------- Text ----------
    {"txt", AssetType::Text},
    {"md", AssetType::Text},

    // ---------- Config ----------
    {"json", AssetType::Config},
    {"yaml", AssetType::Config},
    {"yml", AssetType::Config},
    {"ini", AssetType::Config},
    {"toml", AssetType::Config},

    // ---------- Pictures ----------
    {"png", AssetType::Picture},
    {"jpg", AssetType::Picture},
    {"jpeg", AssetType::Picture},
    {"bmp", AssetType::Picture},
    {"tga", AssetType::Picture},
    {"hdr", AssetType::Picture},
    {"dds", AssetType::Picture},

    // ---------- Audio ----------
    {"wav", AssetType::Audio},
    {"ogg", AssetType::Audio},
    {"mp3", AssetType::Audio},
    {"flac", AssetType::Audio},

    // ---------- Shader source ----------
    {"vert", AssetType::ShaderSource},
    {"frag", AssetType::ShaderSource},
    {"geom", AssetType::ShaderSource},
    {"comp", AssetType::ShaderSource},
    {"glsl", AssetType::ShaderSource},
    {"hlsl", AssetType::ShaderSource},

    // ---------- Shader binary ----------
    {"spv", AssetType::ShaderBinary},
    {"cso", AssetType::ShaderBinary},
    {"dxil", AssetType::ShaderBinary},

    // ---------- Shader includes ----------
    {"inc", AssetType::ShaderInclude},
    {"hlsli", AssetType::ShaderInclude},
    // ---------- Materials ------------
    {"mat", AssetType::Material},
  };

  auto it = s_map.find(path.extension().string());
  if (it == s_map.end())
    return AssetType::Unknown;
  else
    return it->second;
}

std::wstring_view AssetTypeToString(AssetType type) noexcept
{
  switch (type)
  {
    case AssetType::Text:
      return L"Text";
    case AssetType::Config:
      return L"Config";
    case AssetType::Picture:
      return L"Picture";
    case AssetType::Audio:
      return L"Audio";
    case AssetType::ShaderSource:
      return L"ShaderSource";
    case AssetType::ShaderBinary:
      return L"ShaderBinary";
    case AssetType::ShaderInclude:
      return L"ShaderInclude";
    case AssetType::Material:
      return L"Material";
    case AssetType::Unknown:
    default:
      return L"Unknown";
  }
}

AssetType StringToAssetType(std::wstring_view str) noexcept
{
  using Map = std::unordered_map<std::wstring_view, AssetType>;
  static Map m_map{
    {L"Text", AssetType::Text},
    {L"Config", AssetType::Config},
    {L"Picture", AssetType::Picture},
    {L"Audio", AssetType::Audio},
    {L"ShaderSource", AssetType::ShaderSource},
    {L"ShaderBinary", AssetType::ShaderBinary},
    {L"ShaderInclude", AssetType::ShaderInclude},
    {L"Material", AssetType::Material},
  };

  auto it = m_map.find(str);
  if (it != m_map.end())
    return it->second;
  return AssetType::Unknown;
}

} // namespace GameFramework::details
