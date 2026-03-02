#include "ShaderFile.hpp"

#include <cassert>
#include <span>

#include <Files/FileManager.hpp>
#include <GameFramework.hpp>
#include <Utility/Crypto.hpp>
#include <Utility/ProcessRunner.hpp>

namespace
{
std::filesystem::path MakeCachedShaderPath(const std::filesystem::path & shaderPath)
{
  static const std::filesystem::path s_shadersCache("./.shaders");
  using StringMap = std::unordered_map<std::wstring_view, std::wstring_view>;
  static const StringMap extensionReplacement{
    {L".frag", L"_frag.spv"}, // for fragment shader
    {L".vert", L"_vert.spv"}, // for vertex shader
    {L".geom", L"_geom.spv"}, // for geometry shader
  };
  GameFramework::Uuid checksum = GameFramework::Checksum(shaderPath.parent_path());
  std::filesystem::path filename = shaderPath.filename();
  auto it = extensionReplacement.find(filename.extension().wstring());
  assert(it != extensionReplacement.end());
  filename.replace_extension("");
  filename.concat(it->second);

  return std::filesystem::absolute(s_shadersCache / checksum.ToString() / filename);
}
} // namespace

namespace RenderPlugin
{

size_t ShaderFile::ReadBinary(GameFramework::IBinaryFileReader & stream, ShaderFile & file)
{
  file.m_path = stream.FullPath();
  file.m_data.resize(stream.Size() / sizeof(uint32_t));
  return stream.Read(std::as_writable_bytes(std::span{file.m_data}));
}

size_t ShaderFile::ReadText(GameFramework::ITextFileReader & stream, ShaderFile & result)
{
  auto cachedPath = ::MakeCachedShaderPath(stream.FullPath());
  if (!std::filesystem::exists(cachedPath))
  {
    std::error_code ec1;
    std::filesystem::create_directories(cachedPath.parent_path(), ec1);
    std::string compileErr;
    std::string compileOut;
    auto ec = GameFramework::ProcessRunner()
                .AddArgument("glslc")
                .AddArgument(stream.FullPath().string())
                .AddArgument("-o")
                .AddArgument(cachedPath.string())
                .Run(compileOut, compileErr);
    if (ec)
    {
      GameFramework::Log(GameFramework::LogMessageType::Error, "Failed to compile shader - ",
                         stream.FullPath(), " - ", ec.message());
    }

    if (!compileErr.empty())
    {
      GameFramework::Log(GameFramework::LogMessageType::Error, "Failed to compile shader - ",
                         stream.FullPath(), " - ", compileErr);
    }
  }

  if (auto ptr = GameFramework::GetFileManager().OpenReadBinary(cachedPath))
    return ShaderFile::ReadBinary(*ptr, result);
  return 0;
}

bool ShaderFile::IsReadyToUse() const noexcept
{
  return !m_path.empty() && !m_data.empty();
}

size_t ShaderFile::Hash() const noexcept
{
  return std::hash<std::filesystem::path>{}(m_path);
}

bool ShaderFile::operator==(const ShaderFile & rhs) const noexcept
{
  return m_path == rhs.m_path;
}

} // namespace RenderPlugin
