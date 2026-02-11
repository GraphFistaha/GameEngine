#pragma once
#include <vector>

#include <Files/FileStream.hpp>

namespace RenderPlugin
{

class ShaderFile final
{
public:
  ShaderFile() = default;
  ~ShaderFile() = default;
  std::filesystem::path GetPath() const noexcept { return m_path; }
  const std::vector<uint32_t> & GetSpirV() const & noexcept { return m_data; }

  bool IsEmpty() const noexcept;

public:
  static size_t ReadBinary(GameFramework::IFileReader & stream, ShaderFile & file);
  static void WriteBinary(GameFramework::IFileWriter & stream, const ShaderFile & file);

public: //Hashable
  size_t Hash() const noexcept;
  bool operator==(const ShaderFile & rhs) const noexcept;

private:
  std::filesystem::path m_path;
  std::vector<uint32_t> m_data;
};

} // namespace RenderPlugin
