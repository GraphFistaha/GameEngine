#pragma once
#include <GameFramework_def.h>

#include <memory>
#include <typeindex>

#include <Assets/Asset.hpp>
#include <Utility/Utility.hpp>

namespace GameFramework
{

struct IAssetCache
{
  virtual ~IAssetCache() = default;

  template<typename T>
  std::shared_ptr<T> Load(const Asset * asset, bool async = true)
  {
    return Utils::FastDynamicCast<T>(LoadBase(asset, async));
  }

  template<typename T>
  std::shared_ptr<T> Get(const Asset * asset) const noexcept
  {
    return Utils::FastDynamicCast<T>(GetBase(asset));
  }

  //void FreeMemory(); // to free memory of game

protected:
  virtual std::shared_ptr<IAssetData> LoadBase(const Asset * asset, bool async = true) = 0;
  virtual std::shared_ptr<IAssetData> GetBase(const Asset * asset) const noexcept = 0;
};
using AssetCacheUPtr = std::unique_ptr<IAssetCache>;

struct AssetCacheRegistry
{
  virtual ~AssetCacheRegistry() = default;

  template<typename CacheT>
  void ConstructCache()
  {
    AddCacheByTypeId(typeid(CacheT::CachedAsset), std::make_unique<CacheT>());
  }

  template<typename CacheT>
  void DestroyCache()
  {
    RemoveCacheByTypeId(typeid(CacheT::CachedAsset));
  }

  template<typename CacheT = IAssetCache, typename T = void>
  CacheT * Get() const noexcept
  {
    if constexpr (std::is_same_v<CacheT, IAssetCache>)
    {
      static_assert(!std::is_same_v<T, void>);
      return GetByTypeId(typeid(T));
    }
    else
    {
      return Utils::FastDynamicCast<CacheT>(GetByTypeId(typeid(CacheT::CachedAsset)));
    }
  }

protected:
  virtual void AddCacheByTypeId(std::type_index id, AssetCacheUPtr && cache) = 0;
  virtual AssetCacheUPtr RemoveCacheByTypeId(std::type_index id) = 0;
  virtual IAssetCache * GetByTypeId(std::type_index id) const noexcept = 0;
};

GAME_FRAMEWORK_API AssetCacheRegistry & GetAssetCacheRegistry();

} // namespace GameFramework
