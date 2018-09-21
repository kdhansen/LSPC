#include "lspc/packet.hpp"

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
  uint8_t packet_type = 1;
  std::vector<uint8_t> payload = {11, 22, 00, 33};
  lspc::Packet packet_from_payload(packet_type, payload);

#ifndef __EXCEPTIONS
  if (packet_from_payload.IsDegenerate())
  {
    return 1;
  }
#endif
  // Getting the output as pointer
  auto enc_buffer = packet_from_payload.encodedDataPtr();
  auto enc_size = packet_from_payload.encodedDataSize();

  // Getting the output of the encoding again via the std::vector interface.
  auto enc_buffer_vector = packet_from_payload.encodedBuffer();
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
