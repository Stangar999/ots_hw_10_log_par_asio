#include <boost/asio.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "server.h"

namespace ba = boost::asio;

void print_time() {
  using clock = std::chrono::system_clock;

  auto time_point = clock::now();
  std::time_t time = clock::to_time_t(time_point);
  std::cout << std::ctime(&time) << std::endl;
}

int main(int argc, const char *argv[]) {
  // std::locale::global(std::locale("Rus"));
  setlocale(LC_ALL, "rus");

  try {
    if (argc != 3) {
      std::cerr << "Usage: async_tcp_server <port> <bulk_size>\n";
      return 1;
    }
    std::size_t port = std::stoi(argv[1]);
    std::size_t bulk = std::stoi(argv[2]);

    boost::asio::io_context io_context;
    Server server(io_context, port, bulk);

    io_context.run();
  } catch (const std::exception &ex) {
    std::cerr << "Exception: " << ex.what() << "\n";
  }
  return 0;
}

//  boost::asio::streambuf buf;
//  boost::asio::streambuf::mutable_buffers_type b = buf.prepare(15);
//  // boost::asio::io_context io_context;
//  // boost::asio::ip::tcp::socket s(io_context);
//  // s.receive(b);
//  //  int size = b.size();
//  for (int i = 0; i < 10; ++i) {
//    *(static_cast<char *>(b.data()) + i) = 'i';
//  }
//  buf.commit(b.size());
//  //  for (int i = 0; i < b.size() - 5; ++i) {
//  //    *(static_cast<char *>(b.data()) + i) = 'g';
//  //  }
//  //  buf.commit(b.size() - 5);
//  //  std::string str_b(static_cast<const char *>(b.data()), b.size());
//  //    std::cout << str_b << std::endl;
//  std::ostream is(&buf);
//  is << "gdgd";

//  boost::asio::streambuf::mutable_buffers_type b1 = buf.prepare(7);
//  for (int i = 0; i < 7; ++i) {
//    *(static_cast<char *>(b1.data()) + i) = 't';
//  }
//  buf.commit(b1.size());

//  boost::asio::streambuf::mutable_buffers_type b2 = buf.prepare(5);
//  for (int i = 0; i < 5; ++i) {
//    *(static_cast<char *>(b2.data()) + i) = 'e';
//  }

//  boost::asio::streambuf::mutable_buffers_type b3 = buf.prepare(5);
//  for (int i = 0; i < 5; ++i) {
//    *(static_cast<char *>(b3.data()) + i) = 'z';
//  }
//  //  buf.commit(b2.size());

//  buf.consume(5);

//  buf.consume(200);

//  std::istream os(&buf);
//  std::string str;
//  os >> str;
//  std::cout << str << std::endl;

//  std::istream os1(&buf);
//  std::string str1;
//  os1 >> str1;
//  std::cout << str1 << std::endl;
