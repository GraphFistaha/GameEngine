set(this_target "RenderPlugin_RHI")

add_library(${this_target} SHARED)

target_sources(${this_target}
PRIVATE
	"dllmain.cpp"
	"Constants.hpp"

	"Common/RenderBatch.hpp"
	"Common/PipelineSettings.hpp"
	"Common/Pipeline.hpp"
	"Common/Renderer.hpp"
	"Common/SceneBase.hpp"

	"Resources/Material.hpp"
	"Resources/Material.cpp"
	"Resources/MaterialCache.hpp"
	"Resources/MaterialCache.cpp"
	"Resources/ShaderFile.hpp"
	"Resources/ShaderFile.cpp"
	"Resources/ShadersCache.cpp"
	"Resources/ShadersCache.hpp"
	"Resources/Texture2D.cpp"
	"Resources/Texture2D.hpp"
	"Resources/TextureCache.cpp"
	"Resources/TextureCache.hpp"

	"Devices/InternalDeviceInterface.hpp"
	"Devices/ScreenDevice.cpp"
	"Devices/ScreenDevice.hpp"

	"Render2D/Primitives.hpp"
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

find_package(stb REQUIRED)

target_link_libraries(${this_target}
PRIVATE
	GameFramework
	RHI
	stb::stb
)

target_compile_definitions(${this_target}
PRIVATE
	SHADERS_DIRECTORY=".shaders"
)


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