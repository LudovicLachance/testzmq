#include <iostream>
#include <string>
#include <ludo/Zmq.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
  ludo::Zmq socket;
  socket.connect("tcp://127.0.0.1:5555");

  json message;
  message["result"] = "wow";

  for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
    std::cout << "Sending Hello " << request_nbr << "…" << '\n';
    socket.write(message.dump());
    std::cout << socket.read() << '\n';
  }
  return 0;
}
