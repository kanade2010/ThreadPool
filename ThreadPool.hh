#ifndef _THREADPOOL_HH
#define _THREADPOOL_HH

#include <vector>
#include <utility>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>

#include "Condition.hh"

class ThreadPool{
public:
  static const int kInitThreadsSize = 3;
  enum taskPriorityE { level0, level1, level2, };
  typedef std::function<void()> Task;
  typedef std::pair<taskPriorityE, Task> TaskPair;

  ThreadPool();
  ~ThreadPool();

  void start();
  void stop();
  void addTask(const Task&);
  void addTask(const TaskPair&);

private:
  ThreadPool(const ThreadPool&);//禁止复制拷贝.
  const ThreadPool& operator=(const ThreadPool&);

  struct TaskPriorityCmp
  {
    bool operator()(const ThreadPool::TaskPair p1, const ThreadPool::TaskPair p2)
    {
        return p1.first > p2.first; //first的小值优先
    }
  };

  void threadLoop();
  Task take();

  typedef std::vector<std::thread*> Threads;
  typedef std::priority_queue<TaskPair, std::vector<TaskPair>, TaskPriorityCmp> Tasks;

  Threads m_threads;
  Tasks m_tasks;

  std::mutex m_mutex;
  Condition m_cond;
  bool m_isStarted;
};

#endif

