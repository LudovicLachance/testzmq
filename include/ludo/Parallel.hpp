#pragma once
#include <ludo/Zmq.hpp>
#include <thread>
#include <vector>

namespace ludo {
class Parallel {
  std::string workBind{"inproc://work"};
  std::string resultBind{"inproc://result"};

  void masterThread(std::vector<std::string> works = {},
                    u_int64_t numberThreads = 1) {
    std::thread result = std::thread{&Parallel::resultThread, this};
    ludo::Zmq giveWork;
    giveWork.bind(this->workBind);

    std::vector<std::thread> workers;
    workers.reserve(numberThreads);
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
    killResult.connect(this->resultBind);
    killResult.writeread("die");

    for (auto &worker : workers) {
      worker.join();
    }
    result.join();
  }

  void workThread() {
    ludo::Zmq asker, resulter;
    asker.connect(this->workBind);
    resulter.connect(this->resultBind);
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
    receiveResult.bind(this->resultBind);
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

  std::vector<std::string> run(std::vector<std::string> works = {},
                               u_int64_t numberThreads = 1) {
    this->results.reserve(works.size());
    this->masterThread(works, numberThreads);
    auto tmp = this->results;
    this->results = {};
    return tmp;
  }
};
}  // namespace ludo