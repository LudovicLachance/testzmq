#include <iostream>
#include <string>
#include <ludo/Zmq.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <chrono>

int main() {
  ludo::Zmq socket;
  socket.subscribe("tcp://127.0.0.1:5555", "{\"???");

  auto start = std::chrono::high_resolution_clock::now();

  for (int request_nbr = 0; request_nbr != 1000000; request_nbr++) {
    socket.read();
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "1000000 message in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds\n";

  return 0;
}
