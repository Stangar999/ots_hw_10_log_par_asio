﻿#pragma once

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

  ~FileLogger();

  void UpdateEnd(const CommandHolder &comand_holder) override;

private:
  FileLogger() {
    _pool_threads.emplace_back(&FileLogger::WriteAndWaite, this);
    _pool_threads.emplace_back(&FileLogger::WriteAndWaite, this);
  }

  void WriteAndWaite();

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
  void UpdateEnd(const CommandHolder &comand_holder) override;
  ~OstreamLogger();

private:
  OstreamLogger();
  void WriteConsole();
  std::queue<CommandHolder> _comand_queue;
  std::condition_variable_any _cv;
  std::mutex _m;
  std::thread _worker;
  std::atomic_bool _is_end = false;
};

// class OstreamLogger : public IObserver {
// public:
//  void UpdateEnd(const CommandHolder &comand_holder) override;

// private:
//  static void WriteConsole(std::stop_token stoken);
//  static std::queue<CommandHolder> _comand_queue;
//  static std::condition_variable_any cv;
//  static std::mutex m;
//  static std::jthread th_log_cons;
//};