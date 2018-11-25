#include <chrono>
#include "Condition.hh"

Condition::Condition(std::mutex &mutex)
  :m_mutex(mutex)
{
}

Condition::~Condition()
{
}

void Condition::wait(std::unique_lock<std::mutex>& lock)
{
  m_cond.wait(lock);
}

void Condition::waitForSeconds(double seconds)
{
  const int64_t kNanoSecondsPerSecond = 1000000000;
  int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

  std::unique_lock<std::mutex> lock(m_mutex);
  m_cond.wait_for(lock, std::chrono::nanoseconds(nanoseconds));
}

void Condition::notify()
{
  m_cond.notify_one();
}

void Condition::notifyAll()
{
  m_cond.notify_all();
}

