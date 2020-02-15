#pragma once
#include <ludo/BitsField.hpp>
#include <string>
#include <zmq.hpp>

namespace ludo {

class Zmq {
  static zmq::context_t context;

  zmq::socket_t socket;

 public:
  Zmq() {}
  ~Zmq() {}

  static int io_threads;

  enum class code_t : u_int64_t {
    empty = 0,
    die = 1,
    ok = 2,
    please = 4,
  };

  union convert_code_t {
    char toChar[8];
    code_t toCode;
  };

  static std::string code_t_s(code_t code) {
    convert_code_t converter;
    converter.toCode = code;
    return std::string{converter.toChar, 8};
  }

  static code_t s_code_t(std::string str) {
    convert_code_t converter;
    strncpy(converter.toChar, str.data(), 8);
    return converter.toCode;
  }

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

  code_t code = code_t::empty;

  std::string read(zmq::recv_flags waiting = zmq::recv_flags::none) {
    zmq::message_t request;
    zmq::detail::recv_result_t recv_result =
        this->socket.recv(request, waiting);

    std::string msg =
        std::string{static_cast<char*>(request.data()), recv_result.value()};
    this->code =
        s_code_t(std::string{std::begin(msg), std::next(std::begin(msg), 8)});

    return std::string{std::next(std::begin(msg), 8), std::end(msg)};
  }

  void write(const std::string& text, code_t code = code_t::ok) {
    std::string str{code_t_s(code)};
    str += text;
    this->socket.send(zmq::message_t(std::begin(str), std::end(str)),
                      zmq::send_flags::none);
  }

  std::string writeread(const std::string& text, code_t code = code_t::ok) {
    this->write(text, code);
    return this->read();
  }

  std::string readwrite(const std::string& text, code_t code = code_t::ok) {
    auto result = this->read();
    this->write(text, code);
    return result;
  }
};
}  // namespace ludo

int ludo::Zmq::io_threads = 1;
zmq::context_t ludo::Zmq::context = zmq::context_t{Zmq::io_threads};