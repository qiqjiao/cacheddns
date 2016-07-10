#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

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

struct Options {
  int local_port = 0;
  std::string remote_ip = "0.0.0.0";
  int remote_port = 0;
} options;

void handler(const boost::system::error_code& error, int signal_number) {
  if (!error) {
    std::cout << "Signal " << signal_number << std::endl;
  }
}
void my_handler (int param) {
  std::cout << "Signal " << param << std::endl;
}

int main(int argc, char* argv[]) {
#if 0
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()
      ("help", "Print help messages")
      ("add", "additional options")
      ("like", "this");

    try {
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm); // can throw
      if (vm.count("help")) {
        std::cout << "Basic Command Line Parameter App" << std::endl
                  << desc << std::endl;
        return EXIT_SUCCESS;
      }

      po::notify(vm); // throws on error, so do after help in case
                      // there are any problems
    }
    catch(po::error& e)
    {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      std::cerr << desc << std::endl;
      return EXIT_FAILURE;
    }
#endif

  try {
    boost::asio::io_service io_service;
    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "send") {
            std::string r_addr, r_port, message;
            int l_port = 0;
            std::cin >> r_addr >> r_port >> l_port >> message;
            udp::socket s(io_service, udp::endpoint(udp::v4(), l_port));
            udp::resolver resolver(io_service);
            udp::endpoint endpoint = *resolver.resolve({udp::v4(), r_addr, r_port});
            std::cout << "Sending ping to server [" << endpoint << "]\n";
            s.send_to(boost::asio::buffer(message), endpoint);
        } else if (cmd == "recv") {
            int l_port = 0;
            std::cin >> l_port;
            udp::socket sock(io_service, udp::endpoint(udp::v4(), l_port));
            udp::endpoint sender_endpoint;
            char reply[max_length];
            size_t reply_length = sock.receive_from(
                boost::asio::buffer(reply, max_length), sender_endpoint);
            std::cout << "Reply from [" << sender_endpoint << "] is: "
                      << std::string(reply, reply_length) << "\n";
        }
    }
    //boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
    //signals.async_wait([&io_service](const boost::system::error_code& error,
    //                                 int signal_number) {
    //    if (!error) {
    //        std::cout << "Signal " << signal_number << std::endl;
    //    }
    //});
    io_service.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
