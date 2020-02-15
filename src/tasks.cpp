#include <chrono>
#include <cmath>
#include <iostream>
#include <ludo/Parallel.hpp>
#include <string>

void test(int n) {
  ludo::Parallel workers;
  std::deque<std::string> works;
  int number = 1000000;
  works.resize(number, "work");
  auto start = std::chrono::high_resolution_clock::now();

  auto results = workers.run(works, n);

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << std::to_string(number) << " message with " << n << " worker in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << " milliseconds\n";
}

int main() {
  for (size_t i = 0; i < 7; ++i) {
    test(std::pow(2, i));
  }

  return 0;
}
