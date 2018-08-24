#include "lspc/boost/Socket.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <iomanip>

void handl(const std::vector<uint8_t>& payload)
{
  std::cout << "Handling:";
  std::stringstream ss;
  ss << std::setfill('0');
  for (int i = 0; i < payload.size(); ++i)
  {
      // ss << " " << std::setw(2) << std::hex << 55;
      ss << " 0x" << std::setw(2) << std::hex << static_cast<int>(payload[i]);
  }
  std::cout << ss.str();
  return;
}

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    return false;
  }
  std::string recv_port_name(argv[1]);
  std::string test_port_name(argv[2]);
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
  LSPC::Socket mySocket;
  open_tries = 5;
  while (! mySocket.IsOpen())
  {
    try {
      mySocket.Open(recv_port_name);
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
  mySocket.RegisterCallback(1, handl);

  // send some data
  std::array<uint8_t, 9> send_buffer{0, 1, 6, 4, 11, 22, 33, 2, 44};
  std::size_t bytes_transferred = test_port.write_some(boost::asio::buffer(send_buffer));

  std::this_thread::sleep_for (std::chrono::seconds(1));

  // Close test port
  test_port.cancel();
  test_port.close();
  ioservice.stop();
  ioservice.reset();
  return 0;
}
