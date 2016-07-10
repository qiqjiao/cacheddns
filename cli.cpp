#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: cli <host> <port> send(0)/recv(1)\n";
    return 1;
  }
  bool sender = argv[3][0] == '0';

  try {
    boost::asio::io_service io_service;

    udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver(io_service);
    udp::endpoint endpoint = *resolver.resolve({udp::v4(), argv[1], argv[2]});

    std::cout << "Sending ping to server [" << endpoint << "]\n";
    s.send_to(boost::asio::buffer("ping"), endpoint);

    char reply[max_length];
    udp::endpoint sender_endpoint;
    size_t reply_length = s.receive_from(
        boost::asio::buffer(reply, max_length), sender_endpoint);
    std::cout << "Reply from [" << sender_endpoint << "] is: "
              << std::string(reply, reply_length) << "\n";

    std::string peer_ip, peer_port;
    std::istringstream iss(std::string(reply, reply_length));
    iss >> peer_ip >> peer_port;
    endpoint = *resolver.resolve({udp::v4(), peer_ip, peer_port});

    std::cout << "Sending ping to server [" << endpoint << "]\n";
    s.send_to(boost::asio::buffer("ping"), endpoint);

    while (sender) {
      std::cout << "Sending ping to server [" << endpoint << "]\n";
      s.send_to(boost::asio::buffer("ping"), endpoint);
      sleep(1);
    }

    reply_length = s.receive_from(
        boost::asio::buffer(reply, max_length), sender_endpoint);
    std::cout << "Reply from [" << sender_endpoint << "] is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
