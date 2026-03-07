#pragma once
#include <memory>

#include <Utility/Utility.hpp>

namespace RenderPlugin
{
template<typename PrimT>
struct BaseRenderer;

struct IRenderer
{
  virtual ~IRenderer() = default;

  template<typename PrimitiveT>
  bool SetBatch(const RenderBatch<PrimitiveT> & batch)
  {
    if (auto * renderer = GameFramework::Utils::FastDynamicCast<BaseRenderer<PrimitiveT>>(this))
    {
      return renderer->SetBatchImpl(batch);
    }
    return false;
  }

  virtual void Submit() = 0;
};

using RendererUPtr = std::unique_ptr<IRenderer>;

template<typename PrimT>
struct BaseRenderer : public IRenderer
{
  friend struct IRenderer;

protected:
  virtual bool SetBatchImpl(const RenderBatch<PrimT> & batch) = 0;
};

} // namespace RenderPlugin
