#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <ludo/Parallel.hpp>
#include <string>
#include <unordered_map>

std::string done_one(std::string work) { return work; }
std::string wow_nice(std::string work) { return " wow so nice !!!"; }

static unordered_map<u_int64_t, function<std::string(std::string)>> tasks{
    {0ull, done_one},
    {1ull, wow_nice},
};

void test(int n) {
  int number = 100000;
  std::vector<std::string> works;
  works.resize(number, "work");
  ludo::Parallel workers;
  workers.setCallbacks(tasks);

  auto start = std::chrono::high_resolution_clock::now();
  auto results = workers.run(works, n);
  auto end = std::chrono::high_resolution_clock::now();

  // for (auto &&result : results) {
  //   std::cout << result << '\n';
  // }

  std::cout << std::to_string(number) << " message with " << n << " worker in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << " milliseconds\n";
}

int main() {
  test(std::pow(2, 2));

  return 0;
}
