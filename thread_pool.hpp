
#ifndef _thread_pool_HPP
#define _thread_pool_HPP

#include <vector>
#include <deque>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

//!
//! convenience macro to log with file and line information
//!
#ifdef __SOLA_LOGGING_ENABLED
#define __SOLA_LOG(level, msg) sola::level(msg, __FILE__, __LINE__);
#else
#define __SOLA_LOG(level, msg)
#endif /* __SOLA_LOGGING_ENABLED */

namespace sola {

class logger_iface {
public:
  //! ctor
  logger_iface(void) = default;
  //! dtor
  virtual ~logger_iface(void) = default;

  //! copy ctor
  logger_iface(const logger_iface&) = default;
  //! assignment operator
  logger_iface& operator=(const logger_iface&) = default;

public:
  //!
  //! debug logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void debug(const std::string& msg, const std::string& file, std::size_t line) = 0;

  //!
  //! info logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void info(const std::string& msg, const std::string& file, std::size_t line) = 0;

  //!
  //! warn logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void warn(const std::string& msg, const std::string& file, std::size_t line) = 0;

  //!
  //! error logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void error(const std::string& msg, const std::string& file, std::size_t line) = 0;
};

//!
//! default logger class provided by the library
//!
class logger : public logger_iface {
public:
  //!
  //! log level
  //!
  enum class log_level {
    error = 0,
    warn  = 1,
    info  = 2,
    debug = 3
  };

public:
  //! ctor
  logger(log_level level = log_level::info);
  //! dtor
  ~logger(void) = default;

  //! copy ctor
  logger(const logger&) = default;
  //! assignment operator
  logger& operator=(const logger&) = default;

public:
  //!
  //! debug logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void debug(const std::string& msg, const std::string& file, std::size_t line);

  //!
  //! info logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void info(const std::string& msg, const std::string& file, std::size_t line);

  //!
  //! warn logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void warn(const std::string& msg, const std::string& file, std::size_t line);

  //!
  //! error logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void error(const std::string& msg, const std::string& file, std::size_t line);

private:
  //!
  //! current log level in use
  //!
  log_level m_level;

  //!
  //! mutex used to serialize logs in multithreaded environment
  //!
  std::mutex m_mutex;
};

//!
//! variable containing the current logger
//! by default, not set (no logs)
//!
extern std::unique_ptr<logger_iface> active_logger;

//!
//! debug logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void debug(const std::string& msg, const std::string& file, std::size_t line);

//!
//! info logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void info(const std::string& msg, const std::string& file, std::size_t line);

//!
//! warn logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void warn(const std::string& msg, const std::string& file, std::size_t line);

//!
//! error logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void error(const std::string& msg, const std::string& file, std::size_t line);


class thread_pool{
public:
  typedef std::function<void()> task_t;

  thread_pool(int init_size = 1);
  ~thread_pool();

  void stop();

  void add_task(std::weak_ptr<task_t> task);

private:
  thread_pool(const thread_pool&);//ç¦æ­¢å¤å¶æ·è´.
  const thread_pool& operator=(const thread_pool&);
  
  bool is_started() { return m_is_started; }
  void start();

  void thread_loop();
  std::weak_ptr<thread_pool::task_t>  take();

  typedef std::vector<std::thread*> threads_t;

  typedef std::deque<std::weak_ptr<task_t>> tasks_t;

  int m_init_threads_size;

  threads_t m_threads;
  tasks_t m_tasks;

  std::mutex m_mutex;
  std::condition_variable m_cond;
  bool m_is_started;
};

}
#endif

