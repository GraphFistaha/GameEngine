#include "AssetsRegistry.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <unordered_map>
#include <vector>

#include <Assets/Asset.hpp>
#include <Assets/Utils.hpp>
#include <Files/FileManager.hpp>
#include <GameFramework.hpp>
#include <Utility/StringUtils.hpp>

namespace GameFramework
{

struct AssetsRegistryImpl : public AssetsRegistry
{
  AssetsRegistryImpl() = default;
  virtual ~AssetsRegistryImpl() override = default;

  /// @brief deletes database from memory
  virtual void DeleteDatabase(const std::filesystem::path & path) override;

  /// @brief loads entire database of meta-assets to RAM
  /// @param path to the file of database
  virtual void LoadDatabase(const std::filesystem::path & path) override;

  /// @brief get asset by uuid
  virtual const Asset * GetAsset(const Uuid & uuid) const override;

  /// @brief get asset by path
  virtual const Asset * GetAsset(const std::filesystem::path & path) const override;

private:
  using AssetsContainer = std::vector<Asset>;
  using AssetPtrStack = std::deque<Asset *>;
  std::unordered_map<std::filesystem::path, AssetsContainer> m_assetsModules;
  std::unordered_map<Uuid, Asset *> m_assetsByUuids;
  std::unordered_map<std::filesystem::path, AssetPtrStack> m_assetsByPath;
};

void AssetsRegistryImpl::DeleteDatabase(const std::filesystem::path & directory)
{
  auto modulePath = std::filesystem::canonical(directory);
  auto it = m_assetsModules.find(modulePath);
  if (it == m_assetsModules.end())
  {
    Log(LogMessageType::Error, "Assets module - ", modulePath, "is not uploaded yet");
    return;
  }

  for (auto && asset : it->second)
  {
    m_assetsByUuids.erase(asset.GetUUID());
    auto it2 = m_assetsByPath.find(asset.GetPath());
    if (it2 == m_assetsByPath.end())
    {
      throw std::runtime_error("Asset couldn't be found by path");
    }
    std::erase(it2->second, &asset);
  }
  m_assetsModules.erase(it);
}

void AssetsRegistryImpl::LoadDatabase(const std::filesystem::path & path)
{
  bool res = false;
  std::filesystem::path modulePath = std::filesystem::canonical(path);
  std::filesystem::path moduleFilename = modulePath / s_assetsModuleFilename;
  auto [it, inserted] = m_assetsModules.insert({modulePath, {}});
  if (!inserted)
  {
    Log(LogMessageType::Error, "Assets module - ", modulePath, " - is already uploaded");
    return;
  }

  // read .assetList file (in csv format)
  if (TextFileReaderUPtr reader = GetFileManager().OpenReadText(moduleFilename))
  {
    AssetsContainer assets;

    std::wstring header;
    reader->ReadLine(header);

    std::wstring line;
    while (size_t readSymbols = reader->ReadLine(line))
    {
      std::wstring_view v{line};
      std::vector<std::wstring_view> data = Utils::Split(v, L';');
      std::optional<Uuid> uuid = Uuid::MakeFromString(data[0]);
      AssetType type = details::StringToAssetType(data[1]);
      std::filesystem::path asssetPath = data[2];

      if (uuid.has_value() && type != AssetType::Unknown)
      {
        assets.emplace_back(type, *uuid, asssetPath);
      }
      else
      {
        Log(LogMessageType::Warning, "unknown asset - ", line);
      }

      line.clear();
    }
    it->second = std::move(assets);
    res = true;
  }
  else
  {
    Log(LogMessageType::Error, L"Couldn't find ", s_assetsModuleFilename, L"in asset's folder - ",
        path);
  }

  // mount folder in virtual filesystem and fill internal containers
  if (res)
  {
    GetFileManager().Mount("/", CreateDirectoryMountPoint(modulePath));
    for (auto && asset : it->second)
    {
      m_assetsByUuids.insert({asset.GetUUID(), &asset});
      auto [it2, _] = m_assetsByPath.insert({asset.GetPath(), {}});
      it2->second.push_back(&asset);
    }
  }
}

const Asset * AssetsRegistryImpl::GetAsset(const Uuid & uuid) const
{
  auto it = m_assetsByUuids.find(uuid);
  if (it == m_assetsByUuids.end())
    return nullptr;

  return it->second;
}

const Asset * AssetsRegistryImpl::GetAsset(const std::filesystem::path & path) const
{
  auto it = m_assetsByPath.find(path);
  if (it == m_assetsByPath.end())
    return nullptr;
  assert(!it->second.empty());
  return it->second.empty() ? nullptr : it->second.back();
}

} // namespace GameFramework

namespace GameFramework
{

GAME_FRAMEWORK_API AssetsRegistry & GetAssetsRegistry()
{
  static AssetsRegistryImpl s_instance;
  return s_instance;
}


} // namespace GameFramework
