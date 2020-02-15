#include <chrono>
#include <cmath>
#include <iostream>
#include <ludo/Parallel.hpp>
#include <string>

void test(int n) {
  ludo::Parallel workers;
  std::vector<std::string> works;
  int number = 100000;
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
  test(std::pow(2, 2));
  // ludo::Zmq::code_t thecode = ludo::Zmq::code_t::empty;
  using bcode_t = ludo::BitsField::Runtime<ludo::Zmq::code_t>;
  // thecode = bcode_t::set(thecode, ludo::Zmq::code_t::ok);
  // std::cout << bcode_t::isset(thecode, ludo::Zmq::code_t::ok) << '\n';
  ludo::Zmq::code_t code = ludo::Zmq::code_t::die;
  std::string str = ludo::Zmq::code_t_s(code);
  std::cout << bcode_t::isset(ludo::Zmq::s_code_t(str), ludo::Zmq::code_t::please) << '\n';

  return 0;
}
