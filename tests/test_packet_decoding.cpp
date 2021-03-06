#include "lspc/Packet.hpp"

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
  std::vector<uint8_t> enc_buffer;
  enc_buffer.reserve(argc - 1);
  for (int i = 1; i < argc; ++i)
  {
    enc_buffer.push_back(atoi(argv[i]));
  }

  std::unique_ptr<lspc::Packet> packet_from_buffer;
  try
  {
    packet_from_buffer = std::unique_ptr<lspc::Packet>(new lspc::Packet(enc_buffer));
  }
  catch(...)
  {
    return EXIT_FAILURE;
  }

  auto payload = packet_from_buffer->payload();
  auto packet_type = packet_from_buffer->packetType();
  std::cout << "Payload:";
  for (auto i : payload)
  {
    std::cout << " " << int(i);
  }
  std::cout << ", Type: " << int(packet_type) << '\n';


  return 0;
}
