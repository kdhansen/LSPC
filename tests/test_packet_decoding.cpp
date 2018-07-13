#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "lspc/lspc_packet.hpp"

int main(int argc, char const *argv[])
{
  std::vector<uint8_t> enc_buffer;
  enc_buffer.reserve(argc - 1);
  for (int i = 1; i < argc; ++i)
  {
    enc_buffer.push_back(atoi(argv[i]));
  }

  LSPC::Packet packet_from_buffer(enc_buffer);

  auto payload = packet_from_buffer.Payload();
  auto packet_type = packet_from_buffer.PacketType();
  std::cout << "Payload:";
  for (auto i : payload)
  {
    std::cout << " " << int(i);
  }
  std::cout << ", Type: " << int(packet_type) << '\n';


  return 0;
}
