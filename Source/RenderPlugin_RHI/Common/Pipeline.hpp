#pragma once
#include <Common/PipelineSettings.hpp>
#include <Common/RenderBatch.hpp>

namespace RenderPlugin
{

template<typename PrimT>
using Pipeline = std::pair<PipelineSettings, RenderBatch<PrimT>>;

} // namespace RenderPlugin
