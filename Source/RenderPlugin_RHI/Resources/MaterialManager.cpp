#include "MaterialManager.hpp"

#include <mutex>
#include <queue>
#include <unordered_map>

namespace RenderPlugin
{

struct MaterialManagerImpl : public MaterialManager
{
  MaterialManagerImpl();
  virtual ~MaterialManagerImpl() override = default;

public: // MaterialManager
  virtual const Material * LoadMaterial(const GameFramework::Asset * asset) override;
  virtual void DoTasks() override;

private:
  Material m_nullMaterial;
  std::mutex m_uploadLock;
  std::vector<const GameFramework::Asset *> m_tasksToUpload;
  std::unordered_map<GameFramework::Uuid, Material> m_uploadedMaterials;
};

MaterialManagerImpl::MaterialManagerImpl()
  : m_nullMaterial(nullptr)
{
}

const Material * MaterialManagerImpl::LoadMaterial(const GameFramework::Asset * asset)
{
  if (asset == nullptr)
    return &m_nullMaterial;
  std::lock_guard lk{m_uploadLock};
  auto it = m_uploadedMaterials.find(asset->GetUUID());
  if (it == m_uploadedMaterials.end())
  {
    {
      m_tasksToUpload.push_back(asset);
    }
    return &m_nullMaterial;
  }
  else
  {
    return &it->second;
  }
}

void MaterialManagerImpl::DoTasks()
{
  std::lock_guard lk{m_uploadLock};
  for (auto * asset : m_tasksToUpload)
  {
    auto [it, inserted] = m_uploadedMaterials.insert({asset->GetUUID(), Material()});
    if (inserted)
    {
      if (auto && stream = GameFramework::GetFileManager().OpenReadText(asset->GetPath()))
        stream->ReadValue(it->second);
    }
  }

  m_tasksToUpload.clear();
}


MaterialManager & GetMaterialManager()
{
  static MaterialManagerImpl s_manager;
  return s_manager;
}


} // namespace RenderPlugin
