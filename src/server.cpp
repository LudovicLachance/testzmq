#include <iostream>
#include <string>
#include <ludo/Zmq.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
  ludo::Zmq socket;
  socket.publish("tcp://*:5555");

  json message;
  message["???"] = "nice";


  while (true) {
    socket.write(message.dump());
  }
  return 0;
}
