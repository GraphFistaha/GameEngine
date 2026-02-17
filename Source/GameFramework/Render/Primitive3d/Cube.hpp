#pragma once
#include <Assets/AssetSlot.hpp>
#include <Game/Math.hpp>
#include <Render/RenderPrimitive.hpp>
#include <Utility/Uuid.hpp>

namespace GameFramework::Render
{
struct GAME_FRAMEWORK_API Cube : public IRenderPrimitive
{
  Cube();
  explicit Cube(const Vec3f & pos, Uuid material);

public:
  const Mat4f & GetTransform() const & noexcept;
  const Asset * GetMaterial() const noexcept;

public: // IHashable
  size_t Hash() const noexcept;

private:
  Mat4f m_transform;
  AssetSlot m_material{AssetType::Material};
};

// concepts
static_assert(Hashable<Cube>);

} // namespace GameFramework::Render
