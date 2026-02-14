#include "ShaderFile.hpp"

#include <cassert>
#include <fstream>
#include <span>

namespace RenderPlugin
{

size_t ShaderFile::ReadBinary(GameFramework::IBinaryFileReader & stream, ShaderFile & file)
{
  file.m_path = stream.FullPath();
  file.m_data.resize(stream.Size() / sizeof(uint32_t));
  return stream.Read(std::as_writable_bytes(std::span{file.m_data}));
}

void ShaderFile::WriteBinary(GameFramework::IBinaryFileWriter & stream, const ShaderFile & file)
{
  assert(false); // You can't write shaders
}

bool ShaderFile::IsEmpty() const noexcept
{
  return m_data.empty();
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
