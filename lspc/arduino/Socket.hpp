#ifndef LSPC_ARDUINO_HPP
#define LSPC_ARDUINO_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <vector>

#include <Arduino.h>

#include "lspc/Packet.hpp"
#include "lspc/Serializable.hpp"
#include "lspc/SocketBase.hpp"


namespace lspc
{

class Socket : public SocketBase
{
public:
  Socket(){};

  using SocketBase::send;

  // Send a package with lspc
  //
  // @brief Sends a packaged buffer over the USB serial link.
  //
  // @param type The message type. This is user specific; any type between 1-255.
  // @param payload A vector with the serialized payload to be sent.
  //
  // @return True if the packet was sent.
  bool send(uint8_t type, const std::vector<uint8_t> &payload) override
  {
    Packet outPacket(type, payload);

    // Send it
    if (outPacket.encodedDataSize() ==
        Serial.write(outPacket.encodedDataPtr(), outPacket.encodedDataSize()))
      return true;
    else
      return false;
  };


  // Process incoming data on serial link
  //
  // @brief Reads the serial buffer and dispatches the received payload to the
  // relevant message handling callback function.
  void processSerial()
  {
    size_t bytecount = 0;
    while (Serial.available() && bytecount < 10)
    {
      processIncomingByte(Serial.read());
      bytecount++;
    }
    return;
  };

};

} // namespace lspc

#endif // LSPC_ARDUINO_HPP
