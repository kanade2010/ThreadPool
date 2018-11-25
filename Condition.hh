#ifndef _CONDITION_HH
#define _CONDITION_HH

#include <mutex>
#include <condition_variable>

class Condition{
public:
  explicit Condition(std::mutex &mutex);
  ~Condition();
  void wait(std::unique_lock<std::mutex>& lock);
  void waitForSeconds(double seconds);
  void notify();
  void notifyAll();
private:
  Condition(const Condition&);
  const Condition& operator=(const Condition&);

  std::mutex &m_mutex;
  std::condition_variable m_cond;
};

#endif

