#include <chrono>
#include <cmath>
#include <iostream>
#include <ludo/Parallel.hpp>
#include <string>

void test(int n) {
  ludo::Parallel workers;
  std::vector<std::string> works;
  int number = 10000;
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
  for (size_t i = 0; i < 1; ++i) {
    test(std::pow(2, i+2));
  }

  return 0;
}
