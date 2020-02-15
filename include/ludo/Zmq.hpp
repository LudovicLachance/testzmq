#pragma once
#include <string>
#include <zmq.hpp>

namespace ludo {
class Zmq {
  static int io_threads;
  static zmq::context_t context;

  zmq::socket_t socket;

 public:
  Zmq() {}
  ~Zmq() {}

  void bind(const std::string& url) {
    this->socket = zmq::socket_t{this->context, ZMQ_REP};
    this->socket.bind(url);
  }

  void connect(const std::string& url) {
    this->socket = zmq::socket_t{this->context, ZMQ_REQ};
    this->socket.connect(url);
  }

  void subscribe(const std::string& url, const std::string& filter = "") {
    this->socket = zmq::socket_t{this->context, ZMQ_SUB};
    this->socket.connect(url);
    this->socket.setsockopt(ZMQ_SUBSCRIBE, filter.c_str(), filter.size());
  }

  void publish(const std::string& url) {
    this->socket = zmq::socket_t{this->context, ZMQ_PUB};
    this->socket.bind(url);
  }

  std::string read(zmq::recv_flags waiting = zmq::recv_flags::none) {
    zmq::message_t request;
    zmq::detail::recv_result_t recv_result =
        this->socket.recv(request, waiting);

    return std::string{static_cast<char*>(request.data()), recv_result.value()};
  }

  void write(const std::string& text) {
    this->socket.send(zmq::message_t(text.c_str(), text.size()),
                      zmq::send_flags::none);
  }

  std::string writeread(const std::string& text) {
    this->write(text);
    return this->read();
  }

  std::string readwrite(const std::string& text) {
    auto result = this->read();
    this->write(text);
    return result;
  }
};
}  // namespace ludo

int ludo::Zmq::io_threads = 1;
zmq::context_t ludo::Zmq::context = zmq::context_t{Zmq::io_threads};