#pragma once

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>

#include "observer.h"

// Singleton
class FileLogger : public IObserver {
 public:
  static std::shared_ptr<FileLogger> GetFileLogger();
  static void Deinit();

  ~FileLogger();

  void UpdateEnd(const CommandHolder &comand_holder) override;

 private:
  FileLogger() {
    _pool_threads.emplace_back(&FileLogger::WriteAndWaite, this);
    _pool_threads.emplace_back(&FileLogger::WriteAndWaite, this);
  }

  void WriteAndWaite();

  static std::shared_ptr<FileLogger> _file_logger;
  static std::mutex _mx;

  std::queue<CommandHolder> _comand_queue;
  std::vector<std::thread> _pool_threads;
  std::mutex _m_unique_path;
  std::mutex _m_queue;
  std::condition_variable _cv;
  std::atomic_bool _is_end = false;
  std::atomic_uint _counter = 1;
};

// Singleton
class OstreamLogger : public IObserver {
 public:
  static std::shared_ptr<OstreamLogger> GetOstreamLogger();
  static void Deinit();

  void UpdateEnd(const CommandHolder &comand_holder) override;
  ~OstreamLogger();

 private:
  OstreamLogger();
  void WriteConsole();

  inline static std::shared_ptr<OstreamLogger> _ostream_logger = nullptr;
  inline static std::mutex _mx;

  std::queue<CommandHolder> _comand_queue;
  std::condition_variable_any _cv;
  std::mutex _m;
  std::thread _worker;
  std::atomic_bool _is_end = false;
};
