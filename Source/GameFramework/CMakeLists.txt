set(this_target "GameFramework")

add_library(${this_target} SHARED)

target_sources(${this_target}
PUBLIC
	"GameFramework.hpp"
	"PluginInterfaces/GamePlugin.hpp"
	"PluginInterfaces/RenderPlugin.hpp"
	"PluginInterfaces/WindowsPlugin.hpp"
PRIVATE
	"GameFramework_def.h"
	"GameFramework.cpp"
	"PluginInterfaces/GamePlugin.cpp"

	"Utility/Formatter.hpp"
	"Utility/Storage.hpp"
	"Utility/TypeMapping.hpp"
	"Utility/Heterogeneous.hpp"
	"Utility/StringUtils.hpp"
	"Utility/Random.hpp"
	"Utility/Uuid.hpp"
	"Utility/Uuid.cpp"
	"Utility/Utility.hpp"
	"Utility/Hash.hpp"
	"Utility/ProcessRunner.cpp"
	"Utility/ProcessRunner.hpp"
	"Utility/Crypto.cpp"
	"Utility/Crypto.hpp"

	"Plugin/Plugin.cpp"
	"Plugin/Plugin.hpp"

	"Game/Signal.cpp" 
	"Game/Signal.hpp"
	"Game/Time.cpp"
	"Game/Time.hpp"
	"Game/Math.hpp"
	"Game/Async.cpp"
	"Game/Async.hpp"

	"Files/FileStream.hpp"
	"Files/MountPoint.hpp"
	"Files/FileManager.hpp"
	"Files/FileManager.cpp"
	"Files/StandardFileStream.cpp"
	"Files/DirectoryMountPoint.cpp"
	
	"Assets/Asset.cpp"
	"Assets/Asset.hpp"
	"Assets/AssetsRegistry.cpp"
	"Assets/AssetsRegistry.hpp"
	"Assets/Utils.cpp"
	"Assets/Utils.hpp"
	"Assets/AssetSlot.cpp"
	"Assets/AssetSlot.hpp"
	"Assets/AssetCache.cpp" 
	"Assets/AssetCache.hpp" 

	"Render/Primitive2d/Rect2d.cpp"
	"Render/Primitive2d/Rect2d.hpp"
	"Render/Primitive3d/Cube.hpp"
	"Render/Primitive3d/Cube.cpp"
	"Render/Primitive3d/Camera.cpp"
	"Render/Primitive3d/Camera.hpp"
	"Render/Color.hpp"
	"Render/RenderPrimitive.hpp"
	"Render/Scene2d.hpp"
	"Render/Scene3d.hpp"

	"Input/Input.hpp"
	"Input/InputDevice.hpp"
	"Input/InputDevice.cpp"
	"Input/InputBackend.hpp"
	"Input/InputProcessor.hpp" 
	"Input/ButtonProcessor.cpp" 
	"Input/AxisProcessor.cpp"
	"Input/InputController.hpp"
	"Input/InputController.cpp" 
	"Input/InputQueue.cpp"
	"Input/InputQueue.hpp"
	"Input/BindingParser.cpp"
	"Input/BindingParser.hpp"
)

if (MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(${this_target}
PRIVATE
	-DGAME_FRAMEWORK_BUILD
	-DSAFE_INHERITED_CAST
)

target_include_directories(${this_target}
	PUBLIC
		"${SOURCE_DIR}/GameFramework"
)
add_subdirectory(Tests)

find_package(dylib REQUIRED)
find_package(concurrentqueue REQUIRED)
find_package(glm REQUIRED)
find_package(stduuid REQUIRED)
find_package(reproc++ REQUIRED)
find_package(cryptopp REQUIRED)

target_link_libraries(${this_target}
PRIVATE
	dylib::dylib
	concurrentqueue::concurrentqueue
	glm::glm
	stduuid::stduuid
	reproc::reproc
	cryptopp::cryptopp
)

install(TARGETS ${this_target}
  EXPORT ${this_target}
  RUNTIME DESTINATION Bin
  LIBRARY DESTINATION Lib
  ARCHIVE DESTINATION Lib
  INCLUDES DESTINATION include
)