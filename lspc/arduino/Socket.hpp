#ifndef LSPC_ARDUINO_HPP
#define LSPC_ARDUINO_HPP

#include <Arduino.h>
#include "lspc/Packet.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <vector>

namespace LSPC
{

class Socket
{
  // Members for receiving data on the serial link.
  uint8_t incoming_length = 0;
  std::vector<uint8_t> incoming_data;

  // FSM for receiving
  enum class LookingFor {header, type, length, data};
  LookingFor fsr_state = LookingFor::header;

  // Map of callback functions to handle the incoming messages.
  std::map<uint8_t, void (*)(const std::vector<uint8_t>&)> type_handlers;

public:
  Socket(){};

  // Send a package with lspc
  //
  // @brief Sends a packaged buffer over the USB serial link.
  //
  // @param type The message type. This is user specific; any type between 1-255.
  // @param payload A vector with the serialized payload to be sent.
  //
  // @return True if the packet was sent.
  bool Send(uint8_t type, const std::vector<uint8_t> &payload)
  {
    Packet outPacket(type, payload);

    // Send it
    if (outPacket.EncodedDataSize() ==
        Serial.write(outPacket.EncodedDataPtr(), outPacket.EncodedDataSize()))
      return true;
    else
      return false;
  };

  // Process incoming data on serial link
  //
  // @brief Reads the serial buffer and dispatches the received payload to the
  // relevant message handling callback function.
  void ProcessSerial()
  {
    uint8_t incoming_byte;
    size_t bytecount = 0;
    while (Serial.available() && bytecount < 10)
    {
      incoming_byte = Serial.read();
      bytecount++;

      switch (fsr_state)
      {
        case LookingFor::header:
          if (incoming_byte == 0x00)
          {
            incoming_data.push_back(incoming_byte);
            fsr_state = LookingFor::type;
          }
          break;
        case LookingFor::type:
          if (incoming_byte != 0x00)
          {
            incoming_data.push_back(incoming_byte);
            fsr_state = LookingFor::length;
          }
          break;
        case LookingFor::length:
          incoming_length = incoming_byte;
          incoming_data.push_back(incoming_byte);
          fsr_state = LookingFor::data;
          break;
        case LookingFor::data:
          // Record the data
          incoming_data.push_back(incoming_byte);

          // If we got it all, decode it and invoke the handler
          if (incoming_length + 3 == incoming_data.size())
          {
            Packet inPacket(incoming_data);
            auto handler_it = type_handlers.find(inPacket.PacketType());
            if (handler_it != type_handlers.end())
            {
              handler_it->second(inPacket.Payload());
            }
            else
            {
              // We didn't find the handler.
            }
            // Reset to receive the next.
            fsr_state = LookingFor::header;
            incoming_data.clear();
          }
          break;
      }
    }
    return;
  };

  // Register a callback to handle incoming messages
  //
  // @param type The message type to handle. The type is user specific; any
  // number 1-255.
  // @param handler Callback function of the form void
  // callback(uint8_t* payload, size_t len). With payload being a buffer
  // containing the serialized payload and len the lenght of the payload.
  //
  // @return True if the registration succeeded.
  bool RegisterCallback(uint8_t type, void (*handler)(const std::vector<uint8_t>&))
  {
    if (type == 0x00)
    {
      return false;
    }

    type_handlers[type] = handler;

    return true;
  };
};

} // namespace LSPC

#endif // LSPC_ARDUINO_HPP
