set(this_target "Hello3D")

add_library(${this_target} SHARED
	"dllmain.cpp"
	"FPSCamera.cpp"
	"FPSCamera.hpp"
)

find_package(glm REQUIRED)

target_link_libraries(${this_target}
PUBLIC
	GameFramework
	glm::glm
)

TARGET_ASSETS_DIRECTORY(
	  TARGET ${this_target}
	  DIRECTORY "Data"
	  RESULT_DIRECTORY "Bin/Examples/${this_target}_Data"
)

install(TARGETS ${this_target}
  EXPORT ${this_target}
  RUNTIME DESTINATION Bin/Examples
  LIBRARY DESTINATION Lib/Examples
  ARCHIVE DESTINATION Lib/Examples
  INCLUDES DESTINATION include/Examples
)