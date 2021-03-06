#include "lspc/Packet.hpp"

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

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

  std::unique_ptr<lspc::Packet> packet_from_payload;
#ifdef __EXCEPTIONS
  try
  {
    packet_from_payload = std::unique_ptr<lspc::Packet>(new lspc::Packet(packet_type, payload));
  }
  catch(...)
  {
    return EXIT_FAILURE;
  }
#else
  packet_from_payload = std::unique_ptr<lspc::Packet>(new lspc::Packet(packet_type, payload));
  if (packet_from_payload->isDegenerate())
  {
    return 1;
  }
#endif
  auto enc_buffer = packet_from_payload->encodedDataPtr();
  auto enc_size = packet_from_payload->encodedDataSize();
  for (size_t i = 0; i < enc_size; ++i)
  {
    std::cout << " " << int(enc_buffer[i]);
  }
  std::cout << '\n';

  // Getting the output of the encoding again via the std::vector interface.
  auto enc_buffer_vector = packet_from_payload->encodedBuffer();
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
