#ifndef LSPC_BOOST_HPP
#define LSPC_BOOST_HPP

#include <algorithm>
#include <atomic>
#include <array>
#include <cstdint>
#include <map>
#include <functional>
#include <stdexcept>
#include <vector>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/serial_port.hpp>

#include "lspc/Packet.hpp"

namespace LSPC
{

class Socket
{
  // Members for receiving data on the serial link.
  uint8_t incoming_length = 0;
  std::vector<uint8_t> incoming_data;
  std::array<uint8_t, 1> read_buffer;

  // Serial port
  boost::asio::io_service ioservice;
  boost::asio::serial_port controller_port = boost::asio::serial_port(ioservice);
  std::thread ioservice_thread;
  std::atomic<bool> serial_is_sending;

  // FSM for receiving
  enum class LookingFor {header, type, length, data};
  LookingFor fsr_state = LookingFor::header;

  // Map of callback functions to handle the incoming messages.
  std::map<uint8_t, void (*)(const std::vector<uint8_t>&)> type_handlers;


  // Process incoming data on serial link
  //
  // @brief Reads the serial buffer and dispatches the received payload to the
  // relevant message handling callback function.
  void processSerial(const boost::system::error_code &error,
                     std::size_t bytes_transferred)
  {
    if (error == boost::system::errc::operation_canceled) {
      return;
    } else if (error) {
      throw std::runtime_error(error.message());
    }

    uint8_t incoming_byte = read_buffer[0];

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
          auto handler_it = type_handlers.find(inPacket.packetType());
          if (handler_it != type_handlers.end())
          {
            handler_it->second(inPacket.payload());
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


    // READ THE NEXT PACKET
    // Our job here is done. Queue another read.
    boost::asio::async_read(controller_port, boost::asio::buffer(read_buffer),
                            std::bind(&Socket::processSerial, this, std::placeholders::_1, std::placeholders::_2));

    return;
  };

  void serialWriteCallback(const boost::system::error_code &error,
                           size_t bytes_transferred) {
    serial_is_sending = false;
  }

public:
  Socket()
  : serial_is_sending(false)
  {};

  Socket(const std::string& com_port_name)
  : serial_is_sending(false)
  {
    open(com_port_name);
  };

  ~Socket()
  {
    controller_port.cancel();
    controller_port.close();
    ioservice.stop();
    ioservice.reset();
    ioservice_thread.join();
  };

  void open(const std::string& com_port_name)
  {
    if (controller_port.is_open())
    {
      return;
    }

    controller_port.open(com_port_name);

    // SYNCHRONIZE ON THE PACKAGE HEADER
    boost::asio::async_read(controller_port, boost::asio::buffer(read_buffer),
                            std::bind(&Socket::processSerial, this, std::placeholders::_1, std::placeholders::_2));
    // Start the I/O service in its own thread.
    ioservice_thread = std::thread( [&] {ioservice.run();} );
  }

  bool isOpen()
  {
    return controller_port.is_open();
  }

  // Send a package with lspc
  //
  // @brief Sends a packaged buffer over the USB serial link.
  //
  // @param type The message type. This is user specific; any type between 1-255.
  // @param payload A vector with the serialized payload to be sent.
  //
  // @return True if the packet was sent.
  bool send(uint8_t type, const std::vector<uint8_t> &payload)
  {
    Packet outPacket(type, payload);

    if (!serial_is_sending)
    {
      serial_is_sending = true;
      boost::asio::async_write(controller_port,
                             boost::asio::buffer(outPacket.encodedBuffer()),
                             std::bind(&Socket::serialWriteCallback, this, std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
      return false;
    }
    return true;
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
  bool registerCallback(uint8_t type, void (*handler)(const std::vector<uint8_t>&))
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

#endif // LSPC_BOOST_HPP
