#include <boost/asio.hpp>
#include <exception>
#include <iostream>
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
