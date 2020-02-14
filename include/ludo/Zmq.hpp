#pragma once
#include <string>
#include <zmq.hpp>

struct ludo {
  class Zmq {
    static int io_threads;
    static zmq::context_t context;

    zmq::socket_t socket;

   public:
    Zmq() {}
    ~Zmq() {}

    void bind(std::string url, int type = ZMQ_REP) {
      this->socket = zmq::socket_t{this->context, type};
      this->socket.bind(url);
    }

    void connect(std::string url, int type = ZMQ_REQ) {
      this->socket = zmq::socket_t{this->context, type};
      this->socket.connect(url);
    }

    std::string read(zmq::recv_flags waiting = zmq::recv_flags::none) {
      zmq::message_t request;
      zmq::detail::recv_result_t result = this->socket.recv(request, waiting);

      return std::string{static_cast<char *>(request.data()), result.value()};
    }

    void write(std::string text) {
      this->socket.send(zmq::message_t(text.c_str(), text.size()),
                        zmq::send_flags::none);
    }
  };
};

int ludo::Zmq::io_threads = 2;
zmq::context_t ludo::Zmq::context = zmq::context_t{Zmq::io_threads};