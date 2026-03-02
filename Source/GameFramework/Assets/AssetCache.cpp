#include "AssetCache.hpp"

#include <unordered_map>

namespace GameFramework
{
struct AssetCacheRegistryImpl : public AssetCacheRegistry
{
  AssetCacheRegistryImpl() = default;
  ~AssetCacheRegistryImpl() = default;

public:
  virtual void AddCacheByTypeId(std::type_index id, AssetCacheUPtr && cache) override;
  virtual AssetCacheUPtr RemoveCacheByTypeId(std::type_index id) override;
  virtual IAssetCache * GetByTypeId(std::type_index id) const noexcept override;

private:
  std::unordered_map<std::type_index, AssetCacheUPtr> m_caches;
};

void AssetCacheRegistryImpl::AddCacheByTypeId(std::type_index id, AssetCacheUPtr && cache)
{
  m_caches.insert({id, std::move(cache)});
}

AssetCacheUPtr AssetCacheRegistryImpl::RemoveCacheByTypeId(std::type_index id)
{
  auto it = m_caches.find(id);
  AssetCacheUPtr res = it == m_caches.end() ? nullptr : std::move(it->second);
  m_caches.erase(it);
  return res;
}

IAssetCache * AssetCacheRegistryImpl::GetByTypeId(std::type_index id) const noexcept
{
  auto it = m_caches.find(id);
  return it == m_caches.end() ? nullptr : it->second.get();
}

GAME_FRAMEWORK_API AssetCacheRegistry & GetAssetCacheRegistry()
{
  static AssetCacheRegistryImpl s_registry;
  return s_registry;
}


} // namespace GameFramework
