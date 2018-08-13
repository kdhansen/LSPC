#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "lspc/lspc_packet.hpp"

int main(int argc, char const *argv[])
{
  uint8_t packet_type = 1;
  std::vector<uint8_t> payload = {11, 22, 00, 33};
  LSPC::Packet packet_from_payload(packet_type, payload);

#ifndef __EXCEPTIONS
  if (packet_from_payload.IsDegenerate())
  {
    return 1;
  }
#endif
  // Getting the output as pointer
  auto enc_buffer = packet_from_payload.EncodedDataPtr();
  auto enc_size = packet_from_payload.EncodedDataSize();

  // Getting the output of the encoding again via the std::vector interface.
  auto enc_buffer_vector = packet_from_payload.EncodedBuffer();
  // The sizes should be equal.
  if (enc_size != enc_buffer_vector.size())
  {
    return 2;
  }
  // Also the content
  for (size_t i = 0; i < enc_size; ++i)
  {
    if (enc_buffer[i] != enc_buffer_vector[i])
    {
      return 3;
    }
  }

  return 0;
}
