#include "ProcessRunner.hpp"

#include <ranges>

#include <GameFramework.hpp>

#include <reproc++/run.hpp>

namespace GameFramework
{
struct ProcessRunner::Impl
{
  std ::vector<std::string> m_args;
};

#define TO_IMPL(field) reinterpret_cast<Impl *>(field)

ProcessRunner::ProcessRunner()
{
  static_assert(sizeof(Impl) <= sizeof(m_private));
  new (m_private) Impl();
}

ProcessRunner::~ProcessRunner()
{
  TO_IMPL(m_private)->~Impl();
}

ProcessRunner & ProcessRunner::AddArgument(const std::string_view & arg)
{
  TO_IMPL(m_private)->m_args.emplace_back(arg.begin(), arg.end());
  return *this;
}

std::error_code ProcessRunner::Run(std::string & stdout_, std::string & stderr_, int timeout)
{
  auto && args = TO_IMPL(m_private)->m_args;
  if (args.empty())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  Log(LogMessageType::Debug, "Run application - ", args[0]);
  reproc::process process;
  std::error_code ec = process.start(args);
  if (ec)
    return ec;

  reproc::sink::string sink_out(stdout_);
  reproc::sink::string sink_err(stderr_);
  ec = reproc::drain(process, sink_out, sink_err);
  if (ec)
    return ec;

  int status = 0;
  std::tie(status, ec) = process.wait(
    timeout == std::numeric_limits<int>::max() ? reproc::infinite : reproc::milliseconds(timeout));
  return ec;
}
} // namespace GameFramework
