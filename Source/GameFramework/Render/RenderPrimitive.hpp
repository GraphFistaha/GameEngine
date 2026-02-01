#pragma once
#include <GameFramework_def.h>

#include <memory>
#include <span>
#include <type_traits>

#include <Utility/Hash.hpp>

namespace GameFramework::Render
{
struct GAME_FRAMEWORK_API IRenderPrimitive : public IHashable
{
  virtual ~IRenderPrimitive() = default;
};

} // namespace GameFramework
