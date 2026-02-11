#pragma once
#include <Common/RenderBatch.hpp>
#include <Render/Primitive3d/Camera.hpp>
#include <Render/Primitive3d/Cube.hpp>

namespace RenderPlugin
{
namespace Dim3D
{
using Camera = GameFramework::Render::Camera;
using Cube = GameFramework::Render::Cube;
using CubeBatch = RenderBatch<Cube>;
//using CubePipeline = Pipeline<Cube, CubeRenderer>;
} // namespace Dim3D
} // namespace RenderPlugin
