#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "lspc/lspc_packet.hpp"

int main(int argc, char const *argv[])
{
  if (2 > argc)
  {
    return false;
  }
  uint8_t packet_type = atoi(argv[1]);

  std::vector<uint8_t> payload;
  payload.reserve(argc - 2);
  for (int i = 2; i < argc; ++i)
  {
    payload.push_back(atoi(argv[i]));
  }

  LSPC::Packet packet_from_payload(packet_type, payload);

  auto enc_buffer = packet_from_payload.EncodedDataPtr();
  auto enc_size = packet_from_payload.EncodedDataSize();
  for (size_t i = 0; i < enc_size; ++i)
  {
    std::cout << " " << int(enc_buffer[i]);
  }
  std::cout << '\n';

  return 0;
}
