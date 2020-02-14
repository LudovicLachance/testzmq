#pragma once
#include <thread>
#include <vector>

struct ludo {
  class Parallel {
    void masterThread() {}
    void workThread() {}
    void resultTread() {}

    std::thread result;

   public:
    Parallel(u_int64_t numberThreads) : result{Parallel::resultTread} {
      std::vector<std::thread> workers{numberThreads,
                                       std::thread{Parallel::workThread}};
      this->masterThread();
    }
  };
}