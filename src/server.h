#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include "async.h"

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
 public:
  session(tcp::socket socket, async::handle_t handle)
      : _socket(std::move(socket)),
        _handle(handle),
        _data(128,
              ' ')  // TODO задаю 128Б, но если входящая команда будет длинее то
                    // она разобьется на несколько команд, что не ожидаемо
  // как получать команды размер которых заранее неизвестен?
  {
  }

  void start() {
    //
    do_read();
  }

 private:
  void do_read() {
    auto self(shared_from_this());
    _socket.async_read_some(
        boost::asio::buffer(_data),
        [this, self](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            async::receive(_handle, _data.data(), length);
            do_read();
          } else if (ec == boost::asio::error::eof) {
            async::disconnect(_handle);
            std::cout << "socket close " << std::endl;
          } else {
            async::disconnect(_handle);
            // std::cout << ec.what();
          }
        });
  }

  tcp::socket _socket;
  async::handle_t _handle;
  std::string _data;
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
            std::make_shared<session>(std::move(socket), handle)->start();
          }
          do_accept();
        });
  }

  tcp::acceptor _acceptor;
  size_t _bulk;
};
