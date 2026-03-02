#pragma once
#include <GameFramework_def.h>

#include <functional>

namespace GameFramework
{

/// @brief process async tasks
struct AsyncJobWorker
{
  using Task = std::function<void()>;
  virtual ~AsyncJobWorker() = default;
  virtual void ScheduleTask(Task && task) = 0;
  virtual void DoTasks() = 0;
};

GAME_FRAMEWORK_API AsyncJobWorker & GetAsyncWorker();

} // namespace GameFramework
