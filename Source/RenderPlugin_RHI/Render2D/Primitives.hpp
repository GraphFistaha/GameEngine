#pragma once
#include <Common/RenderBatch.hpp>
#include <Render/Primitive2d/Rect2d.hpp>

namespace RenderPlugin
{
namespace Dim2D
{
using Rect = GameFramework::Render::Rect2d;
using RectBatch = RenderBatch<Rect>;
} // namespace Dim2D
} // namespace RenderPlugin
