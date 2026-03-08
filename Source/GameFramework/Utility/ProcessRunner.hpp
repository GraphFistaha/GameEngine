#pragma once
#include <GameFramework_def.h>

#include <initializer_list>
#include <string_view>
#include <system_error>

namespace GameFramework
{
struct GAME_FRAMEWORK_API ProcessRunner
{
  ProcessRunner();
  ~ProcessRunner();

  ProcessRunner & AddArgument(const std::string_view & arg);
  //void AddArgument(const std::wstring_view & arg);
  std::error_code Run(std::string & stdout_, std::string & stderr_,
                      int timeout = std::numeric_limits<int>::max());

private:
  struct Impl;
  std::byte m_private[128];

private:
  ProcessRunner(const ProcessRunner & rhs) = delete;
  ProcessRunner & operator=(const ProcessRunner & rhs) = delete;
};


} // namespace GameFramework
