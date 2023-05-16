#include <iostream>
#include <vector>

#include <boost/asio.hpp>

#include "async.h"
#include "server.h"

namespace ba = boost::asio;

void print_time() {
  using clock = std::chrono::system_clock;

  auto time_point = clock::now();
  std::time_t time = clock::to_time_t(time_point);
  std::cout << std::ctime(&time) << std::endl;
}

int main(int argc, const char *argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: async_tcp_server <port> <bulk_size>\n";
      return 1;
    }
    std::size_t port = std::stoi(argv[1]);
    std::size_t bulk = std::stoi(argv[2]);

    //    std::size_t bulk = 5;
    //    std::size_t port = 9999;

    boost::asio::io_context io_context;
    Server server(io_context, port, bulk);

    //    boost::asio::steady_timer timer{io_context,
    //    std::chrono::seconds{5}};
    //    std::cout << "async_wait" << std::endl;
    //    print_time();
    //    // Non-blocking call
    //    timer.async_wait([](const boost::system::error_code &error) {
    //      // boost::asio::error::operation_aborted is a possible error
    //     here
    //      if (errno)
    //        std::cout << "Error! " << error.message() << std::endl;
    //      else
    //        std::cout << "Hello from async timeout!" << std::endl;
    //    });

    io_context.run();
  } catch (const std::exception &ex) {
    std::cerr << "Exception: " << ex.what() << "\n";
  }
  //  std::size_t bulk = 5;
  //  auto h = async::connect(bulk);
  //  auto h2 = async::connect(bulk);
  //  async::receive(h, "1", 1);
  //  async::receive(h2, "1\n", 2);
  //  async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
  //  async::receive(h, "b\nc\nd\n}\n89\n", 11);
  //  async::disconnect(h);
  //  async::disconnect(h2);

  return 0;
}
