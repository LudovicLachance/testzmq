#pragma once
#include <deque>
#include <iostream>
#include <ludo/Zmq.hpp>
#include <thread>
#include <vector>

namespace ludo {
class Parallel {
  void masterThread(std::deque<std::string> works = {},
                    u_int64_t numberThreads = 1) {
    std::thread result = std::thread{&Parallel::resultThread, this};
    ludo::Zmq giveWork;
    giveWork.bind("inproc://work");

    std::vector<std::thread> workers;
    for (size_t i = 0; i < numberThreads; ++i) {
      workers.emplace_back(std::thread{&Parallel::workThread, this});
    }

    for (auto &work : works) {
      std::string request = giveWork.read();
      if (request == "please") {
        giveWork.write(work);
      }
    }

    for (size_t i = 0; i < workers.size(); ++i) {
      giveWork.readwrite("die");
    }

    ludo::Zmq killResult;
    killResult.connect("inproc://result");
    killResult.writeread("die");

    for (auto &worker : workers) {
      worker.join();
    }
    result.join();
  }

  void workThread() {
    ludo::Zmq asker, resulter;
    asker.connect("inproc://work");
    resulter.connect("inproc://result");
    bool shouldDie = false;

    do {
      std::string work = asker.writeread("please");
      shouldDie = work != "die";
      if (shouldDie) {
        resulter.writeread(work + " done!!!");
      }
    } while (shouldDie);
  }

  void resultThread() {
    ludo::Zmq receiveResult;
    receiveResult.bind("inproc://result");
    bool shouldDie = false;

    do {
      std::string result = receiveResult.readwrite("ok");
      shouldDie = result != "die";
      if (shouldDie) {
        this->results.push_back(result);
      }
    } while (shouldDie);
  }

  std::vector<std::string> results;

 public:
  Parallel() = default;
  ~Parallel() = default;

  std::vector<std::string> run(std::deque<std::string> works = {},
                               u_int64_t numberThreads = 1) {
    this->results.resize(works.size());
    this->masterThread(works, numberThreads);
    auto tmp = this->results;
    this->results = {};
    return tmp;
  }
};
}  // namespace ludo