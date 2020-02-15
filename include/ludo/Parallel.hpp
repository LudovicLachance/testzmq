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
    // std::cout << "All started" << '\n';
    while (works.size() > 0) {
      std::string request = giveWork.read();
      if (request == "please") {
        // std::cout << "read please" << '\n';

        giveWork.write(works.front());
        works.pop_front();
      }
    }
    for (size_t i = 0; i < workers.size(); ++i) {
      giveWork.read();
      giveWork.write("die");
    }
    // std::cout << "killed worker" << '\n';

    ludo::Zmq killResult;
    killResult.connect("inproc://result");
    killResult.write("die");
    killResult.read();
    // std::cout << "killed result" << '\n';
    for (auto &worker : workers) {
      worker.join();
    }
    // std::cout << "join worker" << '\n';
    result.join();
    // std::cout << "join result" << '\n';
  }
  void workThread() {
    ludo::Zmq asker;
    asker.connect("inproc://work");
    ludo::Zmq resulter;
    resulter.connect("inproc://result");
    std::string work;
    do {
      asker.write("please");
      work = asker.read();
      if (work != "die") {
        resulter.write(work + " done!!!");
        resulter.read();
      }
    } while (work != "die");
    // std::cout << "worker die" << '\n';
  }
  void resultThread() {
    ludo::Zmq receiveResult;
    receiveResult.bind("inproc://result");
    std::string result;
    do {
      result = receiveResult.read();
      receiveResult.write("ok");
      if (result != "die") {
        this->results.push_back(result);
      }
    } while (result != "die");
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