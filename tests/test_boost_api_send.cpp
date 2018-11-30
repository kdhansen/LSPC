#include "lspc/boost/Socket.hpp"
#include "lspc/Serializable.hpp"

#include <array>
#include <cstdint>
#include <iostream>

class Type1Object : public lspc::Serializable
{
  std::vector<uint8_t> payload_ = {55, 66, 77, 0, 88};
  uint8_t type_ = 1;

public:
  std::vector<uint8_t> serialize() const override
  {
    return payload_;
  }

  uint8_t type() const override
  {
    return type_;
  }
};


int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    return false;
  }
  std::string send_port_name(argv[1]);
  std::string test_port_name(argv[2]);
  // Receiving on COM9 for test
  boost::asio::io_service ioservice;
  boost::asio::serial_port test_port(ioservice);
  int open_tries = 5;
  while (! test_port.is_open())
  {
    try {
      test_port.open(test_port_name);
    }
    catch (boost::system::system_error& e)
    {
      if (0 == --open_tries)
      {
        std::terminate();
      }
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }
  }

  // Code to test
  lspc::Socket mySocket;
  open_tries = 5;
  while (! mySocket.isOpen())
  {
    try {
      mySocket.open(send_port_name);
    }
    catch (boost::system::system_error&)
    {
      if (0 == --open_tries)
      {
        std::terminate();
      }
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }
  }
  std::vector<uint8_t> payload = {11, 22, 33, 0, 44};
  mySocket.send(1, payload);

  // Get data from mySocket.send(1, payload)
  std::array<uint8_t, 258> read_buffer;
  std::size_t bytes_transferred = test_port.read_some(boost::asio::buffer(read_buffer));
  for (int i = 0; i < bytes_transferred; ++i)
  {
    std::cout << " " << int(read_buffer[i]);
  }
  std::cout << '\n';

  Type1Object obj;
  mySocket.send(obj);
  // Get data from mySocket.send(obj);
  bytes_transferred = test_port.read_some(boost::asio::buffer(read_buffer));
  for (int i = 0; i < bytes_transferred; ++i)
  {
    std::cout << " " << int(read_buffer[i]);
  }
  std::cout << '\n';


  // Close test port
  test_port.cancel();
  test_port.close();
  ioservice.stop();
  ioservice.reset();
  return 0;
}
