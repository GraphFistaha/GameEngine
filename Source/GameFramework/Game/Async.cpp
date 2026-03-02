#include "Async.hpp"

#include "concurrentqueue.h"

namespace GameFramework
{
struct AsyncJobWorkerImpl : public AsyncJobWorker
{
  AsyncJobWorkerImpl() = default;

public:
  virtual void ScheduleTask(Task && task) override;
  virtual void DoTasks() override;

private:
  moodycamel::ConcurrentQueue<Task> m_tasks;
};

void AsyncJobWorkerImpl::ScheduleTask(Task && task)
{
  m_tasks.enqueue(std::move(task));
}

void AsyncJobWorkerImpl::DoTasks()
{
  bool hasTask = false;
  do
  {
    Task t;
    hasTask = m_tasks.try_dequeue(t);
    if (hasTask)
    {
      t();
    }
  } while (hasTask);
}

GAME_FRAMEWORK_API AsyncJobWorker & GetAsyncWorker()
{
  static AsyncJobWorkerImpl s_worker;
  return s_worker;
}


} // namespace GameFramework
