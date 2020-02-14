#include <iostream>
#include <string>
#include <ludo/Zmq.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
  ludo::Zmq socket;
  socket.bind("tcp://*:5555");

  json message;
  message["???"] = "nice";


  while (true) {
    std::cout << socket.read() << '\n';
    socket.write(message.dump());
  }
  return 0;
}
