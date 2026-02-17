#pragma once
#include <Resources/Material.hpp>
namespace RenderPlugin
{

struct MaterialManager
{
  virtual ~MaterialManager() = default;

  /// @brief async method to upload material
  /// @param asset - path to material
  /// @return valid pointer on Material if material is uploaded or null Material if not
  virtual const Material * LoadMaterial(const GameFramework::Asset * asset) = 0;

  /// @brief complete all upload tasks
  virtual void DoTasks() = 0;
};


MaterialManager & GetMaterialManager();
} // namespace RenderPlugin
