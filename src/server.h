#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "async.h"

using boost::asio::ip::tcp;
namespace ba = boost::asio;
class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket, async::handle_t handle)
      : _socket(std::move(socket)),
        _handle(handle),
        qwery() {
  }

  void start() {
    do_read();
  }

 private:
  void do_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(
        _socket, qwery, '\n',
        [this, self](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            // static_cast<const char*>(qwery.data().data()), length - 1
            std::string str;
            str.resize(length);
            qwery.sgetn(str.data(), length);  // считываем вместе с '\n'

            async::receive(
                _handle, str.data(),
                str.size() - 1);  // символ конца строки больше не нужен
            // qwery.consume(length);
            do_read();
          } else if (ec == boost::asio::error::eof) {
            async::disconnect(_handle);
            std::cout << "socket close " << std::endl;
          } else {
            async::disconnect(_handle);
            std::cout << ec.message();
          }
        });
  }

  tcp::socket _socket;
  async::handle_t _handle;
  boost::asio::streambuf qwery;
};

class Server {
 public:
  Server(boost::asio::io_context &io_context, std::size_t port,
         std::size_t bulk)
      : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)),
        _bulk(bulk) {
    do_accept();
  }

 private:
  void do_accept() {
    _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
          if (!ec) {
            async::handle_t handle = async::connect(_bulk);
            std::make_shared<Session>(std::move(socket), handle)->start();
          }
          do_accept();
        });
  }

  tcp::acceptor _acceptor;
  size_t _bulk;
};
