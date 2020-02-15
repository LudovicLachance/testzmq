#pragma once
#include <ludo/BitsField.hpp>
#include <ludo/Zmq.hpp>
#include <thread>
#include <vector>

namespace ludo {
class Parallel {
  using code_t = ludo::Zmq::code_t;
  using bcode_t = ludo::BitsField::Runtime<code_t>;
  std::string workBind{"inproc://work"};
  std::string resultBind{"inproc://result"};

  void masterThread(const std::vector<std::string>& works = {},
                    u_int64_t numberThreads = 1) {
    std::thread result = std::thread{&Parallel::resultThread, this};
    ludo::Zmq giveWork;
    giveWork.bind(this->workBind);

    std::vector<std::thread> workers;
    workers.reserve(numberThreads);
    for (size_t i = 0; i < numberThreads; ++i) {
      workers.emplace_back(std::thread{&Parallel::workThread, this});
    }

    for (auto& work : works) {
      std::string request = giveWork.read();
      if (bcode_t::isset(giveWork.code, code_t::please)) {
        giveWork.write(work);
      }
    }

    for (size_t i = 0; i < workers.size(); ++i) {
      giveWork.readwrite("", code_t::die);
    }

    ludo::Zmq killResult;
    killResult.connect(this->resultBind);
    killResult.writeread("", code_t::die);

    for (auto& worker : workers) {
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
      std::string work = asker.writeread("", code_t::please);
      shouldDie = bcode_t::isset(asker.code, code_t::die);
      if (!shouldDie) {
        resulter.writeread(work + " done!!!");
      }
    } while (!shouldDie);
  }

  void resultThread() {
    ludo::Zmq receiveResult;
    receiveResult.bind(this->resultBind);
    bool shouldDie = false;

    do {
      std::string result = receiveResult.readwrite("", code_t::ok);
      shouldDie = bcode_t::isset(receiveResult.code, code_t::die);
      if (!shouldDie) {
        this->results.push_back(result);
      }
    } while (!shouldDie);
  }

  std::vector<std::string> results;

 public:
  Parallel() = default;
  ~Parallel() = default;

  std::vector<std::string> run(const std::vector<std::string>& works = {},
                               u_int64_t numberThreads = 1) {
    // ludo::Zmq::io_threads = numberThreads / 2 + 1;
    this->results.reserve(works.size());
    this->masterThread(works, numberThreads);
    std::vector<std::string> tmp;
    this->results.swap(tmp);
    return tmp;
  }
};
}  // namespace ludo