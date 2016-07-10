#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_service& io_service, unsigned short port)
{
  udp::socket sock(io_service, udp::endpoint(udp::v4(), port));

  char data[max_length];
  udp::endpoint sender_endpoint1;
  size_t length = sock.receive_from(
      boost::asio::buffer(data, max_length), sender_endpoint1);
  std::cout << "Received from [" << sender_endpoint1 << "]:"
            << std::string(data, length) << std::endl;

  udp::endpoint sender_endpoint2;
  length = sock.receive_from(
      boost::asio::buffer(data, max_length), sender_endpoint2);
  std::cout << "Received from [" << sender_endpoint2 << "]:"
            << std::string(data, length) << std::endl;

  std::string ip_port1 = sender_endpoint1.address().to_string() + " " +
                         std::to_string(sender_endpoint1.port());
  std::string ip_port2 = sender_endpoint2.address().to_string() + " " +
                         std::to_string(sender_endpoint2.port());

  std::cout << "Sending clients 1 & 2 each other's ip and port.\n";
  sock.send_to(boost::asio::buffer(ip_port2), sender_endpoint1);
  sock.send_to(boost::asio::buffer(ip_port1), sender_endpoint2);

  std::cout << "Clients 1 & 2 know each other now.\n";
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: svr <port>\n";
    return 1;
  }

  try {
    boost::asio::io_service io_service;
    server(io_service, std::atoi(argv[1]));
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
