set(this_target "RenderPlugin_RHI")

add_library(${this_target} SHARED)

target_sources(${this_target}
PRIVATE
	"dllmain.cpp"
	"Constants.hpp"

	"Common/RenderBatch.hpp"
	"Common/PipelineSettings.hpp"
	"Common/Pipeline.hpp"

	"Resources/Material.hpp"
	"Resources/Material.cpp"
	"Resources/MaterialCache.hpp"
	"Resources/MaterialCache.cpp"
	"Resources/ShaderFile.hpp"
	"Resources/ShaderFile.cpp"
	"Resources/ShadersCache.cpp"
	"Resources/ShadersCache.hpp"

	"Devices/InternalDeviceInterface.hpp"
	"Devices/ScreenDevice.cpp"
	"Devices/ScreenDevice.hpp"

	"Render2D/Scene2D_GPU.cpp" 
	"Render2D/Scene2D_GPU.hpp" 
	"Render2D/Scene2D_CPU.cpp"
	"Render2D/Scene2D_CPU.hpp"
	"Render2D/Renderer/BackgroundRenderer.cpp"
	"Render2D/Renderer/BackgroundRenderer.hpp"
	"Render2D/Renderer/Rect2dRenderer.cpp"
	"Render2D/Renderer/Rect2dRenderer.hpp"

	"Render3D/Primitives.hpp"
	"Render3D/Scene3D_CPU.cpp"
	"Render3D/Scene3D_CPU.hpp"
	"Render3D/Scene3D_GPU.cpp"
	"Render3D/Scene3D_GPU.hpp"
	"Render3D/Renderer/CubeRenderer.cpp"
	"Render3D/Renderer/CubeRenderer.hpp"
)

target_include_directories(${this_target} 
PUBLIC
	"${SOURCE_DIR}/RenderPlugin_RHI"
)

target_link_libraries(${this_target}
PRIVATE
	GameFramework
	RHI
)

target_compile_definitions(${this_target}
PRIVATE
	SHADERS_DIRECTORY=".shaders"
)

#TARGET_PRECOMPILE_SPIRV(
#TARGET ${this_target} 
#SHADERS_DIRECTORY ".shaders"
#SHADERS
#	"Render2D/Shaders/background.vert"
#	"Render2D/Shaders/background.frag"
#	"Render2D/Shaders/rect2d.frag"
#	"Render2D/Shaders/rect2d.vert"

#	"Render3D/Shaders/Cube.frag"
#	"Render3D/Shaders/Cube.vert"
#)

TARGET_ASSETS_DIRECTORY(
	TARGET ${this_target}
	DIRECTORY "./Assets"
	RESULT_DIRECTORY "Bin/RenderPluginData"
)

install(TARGETS ${this_target}
  EXPORT ${this_target}
  RUNTIME DESTINATION Bin
  LIBRARY DESTINATION Lib
  ARCHIVE DESTINATION Lib
  INCLUDES DESTINATION include
)