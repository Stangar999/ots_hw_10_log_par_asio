#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "async.h"

using boost::asio::ip::tcp;
namespace ba = boost::asio;
class session : public std::enable_shared_from_this<session> {
 public:
  session(tcp::socket socket, async::handle_t handle)
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
    // TODO  не работает чтение до '\n', до символа например 'F' работает
    //  во всех примерах async_read_until работает с '\n'
    //  в консоль я пробовал и просто enter нажимать
    //  и явно указывать \n, '\n', "\n", '\\n', "\\n"
    boost::asio::async_read_until(
        _socket, qwery, '\n',
        [this, self](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            /*
            TODO
            так как
            Примечание
             После успешной операции read_until streambuf может содержать
             дополнительные данные за пределами разделителя.
             Приложение обычно оставляет эти данные в streambuf
             для последующей проверки операцией read_until.
             http://charette.no-ip.com:81/programming/doxygen/boost/group__read__until.html

            надо из qwery забирать только часть до разделителя а хвост читать
            при следующей команде

             сейчас сделано через static_cast<const char *>(qwery.data().data())
            что не очень мне нравится поэтому хотел использовать prepare
            ba::streambuf::mutable_buffers_type bufs = qwery.prepare(length);
            // но так не работает
            boost::asio::streambuf qw(bufs);

            // и так не работает
            std::string str(bufs.begin(), bufs.end());

            // тоже не работатет
            std::istream is(&bufs); // не работает
            std::string str;
            std::getline(is, str);

            а как из bufs получить что то с чем можно работать? строку или
            std::istream например?
            */
            async::receive(_handle,
                           static_cast<const char *>(qwery.data().data()),
                           length);
            qwery.consume(length);
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
            std::make_shared<session>(std::move(socket), handle)->start();
          }
          do_accept();
        });
  }

  tcp::acceptor _acceptor;
  size_t _bulk;
};
