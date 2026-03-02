cmake_minimum_required(VERSION 3.13)
include(Utils.cmake)

project(GameFramework C CXX)

# ------------------ options and settings --------------

option(BUILD_EXAMPLES "Build example programs" ON)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/Source)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_C_STANDATD 17)
cmake_policy(SET CMP0060 NEW)
include (${SOURCE_DIR}/assets.cmake)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}" CACHE PATH "Installation directory" FORCE)
endif()
message(STATUS "Install directory is ${CMAKE_INSTALL_PREFIX}")

# -------------------- Install conan packages ----------------------
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conanfile.txt")
    message(STATUS "Copy conanfile.txt into CMake cache")
    configure_file("${PROJECT_SOURCE_DIR}/conanfile.txt" "${CMAKE_BINARY_DIR}/conanfile.txt" COPYONLY)
endif()

include(${SOURCE_DIR}/conan.cmake)

conan_cmake_autodetect(settings RUNTIME static)
message(status ${settings})
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings}
                    OUTPUT_FOLDER ${PROJECT_BINARY_DIR}/ConanResult
)

# TODO: do not find conan_toolchain, make it with CMakePresets
file(GLOB_RECURSE ToolchainFile
     "${PROJECT_BINARY_DIR}/ConanResult/**/conan_toolchain.cmake"
)
get_filename_component(GeneratorsFolder ${ToolchainFile} DIRECTORY)

list(APPEND CMAKE_MODULE_PATH ${GeneratorsFolder})
list(APPEND CMAKE_PREFIX_PATH ${GeneratorsFolder})

# ------------------------- Build projects ------------------------------
add_subdirectory(ThirdParty)
add_subdirectory(Source)
if(${BUILD_EXAMPLES})
	add_subdirectory(Examples)
endif()
