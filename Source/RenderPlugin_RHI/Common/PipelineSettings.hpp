#pragma once
#include <Resources/ShaderFile.hpp>

namespace RenderPlugin
{

/// @brief Generic user-defined settings for a pipeline
/// one pipeline is rendered by one renderer with the settings
struct PipelineSettings final
{
  explicit PipelineSettings(const ShaderFile & file)
    : m_shader(file)
  {
  }

public:
  const ShaderFile & GetShader() const & noexcept { return m_shader; }

public: // Hashable
  size_t Hash() const noexcept { return m_shader.Hash(); }
  bool operator==(const PipelineSettings& rhs) const noexcept { return m_shader == rhs.m_shader; }

private:
  ShaderFile m_shader; ///< fragment shader
};

} // namespace RenderPlugin
