#define USE_GLM
#define GLM_ENABLE_EXPERIMENTAL
#include "Cube.hpp"
#include <Utility/Utility.hpp>

#include "glm/gtx/hash.hpp"

namespace GameFramework::Render
{

Cube::Cube()
{
}

Cube::Cube(const Vec3f & pos, Uuid material)
{
  glm::mat4 m = glm::identity<glm::mat4>();
  m = glm::translate(m, CastToGLM(pos));
  m_transform = CastFromGLM(m);
  m_material.SetAsset(material);
}

const Mat4f & Cube::GetTransform() const & noexcept
{
  return m_transform;
}

const IAsset * Cube::GetMaterial() const noexcept
{
  return m_material.GetAsset();
}

size_t Cube::Hash() const noexcept
{
  size_t seed = 0;
  Utils::combined_hash(seed, CastToGLM(m_transform));
  return seed;
}

} // namespace GameFramework::Render
