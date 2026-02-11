#pragma once
#include <span>
#include <type_traits>
#include <vector>

#include <Common/PipelineSettings.hpp>
#include <Render/RenderPrimitive.hpp>
#include <Resources/Material.hpp>
#include <Utility/Hash.hpp>

namespace RenderPlugin
{

template<typename BatchT, typename PrimT>
concept RenderBatchConcept = requires(BatchT batch, PrimT obj) {
  { batch.Push(obj) };
  { batch.GetObjects() } -> std::convertible_to<std::span<const PrimT>>;
  { batch.ObjectsHash() } -> std::convertible_to<std::size_t>;
  typename BatchT::PrimitiveType;
};

template<typename PrimT>
struct RenderBatch final
{
  RenderBatch() = default;
  using PrimitiveType = PrimT;

  /// @brief push render primitive to draw
  /// @param obj - render primitive
  inline void Push(const PrimT & obj)
  {
    m_objs.push_back(obj);
    GameFramework::Utils::combined_hash(m_hash, obj);
  }

  inline std::span<const PrimT> GetObjects() const noexcept { return m_objs; }

  inline size_t ObjectsHash() const noexcept { return m_hash; }

private:
  std::vector<PrimT> m_objs;
  size_t m_hash = 0;

  //private:
  //  RenderBatch(const RenderBatch &) = delete;
  //  RenderBatch & operator=(const RenderBatch &) = delete;
private:
};


} // namespace RenderPlugin
