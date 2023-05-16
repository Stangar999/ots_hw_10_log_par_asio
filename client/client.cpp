#include <boost/asio.hpp>
#include <iostream>
#include <thread>

namespace ba = boost::asio;

int main(int argc, char *argv[]) {
  std::locale::global(std::locale(""));

  try {
    if (argc != 5) {
      std::cerr << "Usage: <0> <9> <adress> <port>\n";
      return 1;
    }

    std::size_t first_cmd = std::stoi(argv[1]);
    std::size_t last_cmd = std::stoi(argv[2]);
    std::string adress = argv[3];
    std::size_t port = std::stoi(argv[4]);

    ba::io_context io_context;

    ba::ip::tcp::endpoint ep(ba::ip::address::from_string(adress), port);
    ba::ip::tcp::socket sock(io_context);

    sock.connect(ep);

    for (int i = first_cmd; i < last_cmd; ++i) {
      std::string data_ = std::to_string(i) + '\n';
      ba::write(sock, ba::buffer(data_));
    }

  } catch (const boost::system::system_error &ex) {
    std::cout << "boost exception! " << ex.what() << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "std::exception! " << ex.what() << std::endl;
  }

  return 0;
}

//    int type;
//    std::cout << "Type ";
//    std::cin >> type;
//    if (type == 1) {
//      std::size_t first_cmd = std::stoi(argv[1]);
//      std::size_t last_cmd = std::stoi(argv[2]);
//      std::cout << "Set Range ";
//      std::cin >> first_cmd;
//      std::cin >> last_cmd;
//      for (int i = first_cmd; i < last_cmd; ++i) {
//        std::string data_ = std::to_string(i) + '\n';
//        ba::write(sock, ba::buffer(data_));
//        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
//      }
//    } else if (type == 5) {
//      std::size_t first_cmd = std::stoi(argv[1]);
//      std::size_t last_cmd = std::stoi(argv[2]);
//      std::cout << "Set Range ";
//      std::cin >> first_cmd;
//      std::cin >> last_cmd;
//      for (int i = first_cmd; i < last_cmd; ++i) {
//        std::string data_;
//        if (i == 10 || i == 20 || i == 30 || i == 40 || i == 50) {
//          data_ = std::string("{") + '\n';
//        } else if (i == 19 || i == 29 || i == 39 || i == 49 || i == 59) {
//          data_ = std::string("}") + '\n';
//        } else {
//          data_ = std::to_string(i) + '\n';
//        }
//        ba::write(sock, ba::buffer(data_));
//        // std::this_thread::sleep_for(std::chrono::microseconds(1));
//      }
//    } else {
//      while (true) {
//        std::string data_;
//        ;
//        std::cin >> data_;
//        if (data_ == "0") {
//          sock.close();
//          break;
//        }

//        ba::write(sock, ba::buffer(data_));

//        //      std::this_thread::sleep_for(
//        //          std::chrono::duration<int, std::milli>(10000));
//        //      std::cout << "boos";
//      }
//    }
