#include "Asset.hpp"

#include <Assets/Utils.hpp>

#include "Asset.hpp"

namespace GameFramework
{

struct Asset::Impl final
{
  AssetType m_type;
  Uuid m_uuid;
  std::filesystem::path m_path;
  //probably should contain a list of users - another game objects which are use this asset
};

#define TO_IMPL(member)       reinterpret_cast<Impl *>(member)
#define TO_CONST_IMPL(member) reinterpret_cast<const Impl *>(member)

Asset::Asset(AssetType type, const Uuid & uuid, const std::filesystem::path & path)
{
  static_assert(sizeof(Impl) <= sizeof(Asset::m_impl));
  new (m_impl) Impl{type, uuid, path};
}

Asset::~Asset() noexcept
{
 TO_IMPL(m_impl)->~Impl();
}

Asset::Asset(Asset && rhs) noexcept
{
  new (m_impl) Impl(std::move(*TO_IMPL(rhs.m_impl)));
}

Asset & Asset::operator=(Asset && rhs) noexcept
{
  if (this != &rhs)
  {
    new (m_impl) Impl(std::move(*TO_IMPL(rhs.m_impl)));
  }
  return *this;
}

Asset::Asset(const Asset & rhs)
{
  new (m_impl) Impl(*TO_CONST_IMPL(rhs.m_impl));
}

Asset & Asset::operator=(const Asset & rhs)
{
  if (this != &rhs)
  {
    new (m_impl) Impl(*TO_CONST_IMPL(rhs.m_impl));
  }
  return *this;
}

Uuid Asset::GetUUID() const noexcept
{
  return TO_CONST_IMPL(m_impl)->m_uuid;
}

std::filesystem::path Asset::GetPath() const noexcept
{
  return TO_CONST_IMPL(m_impl)->m_path;
}

AssetType Asset::GetType() const noexcept
{
  return TO_CONST_IMPL(m_impl)->m_type;
}

} // namespace GameFramework
