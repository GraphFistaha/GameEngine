#pragma once
#include <GameFramework_def.h>

#include <filesystem>

#include <Utility/Uuid.hpp>

namespace GameFramework
{
GAME_FRAMEWORK_API Uuid Checksum(const std::filesystem::path & path);
}
