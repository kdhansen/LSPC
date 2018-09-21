#include "lspc/arduino/Socket.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include "Arduino.h"

FakeSerial Serial;
LSPC::Socket myLspc;

void handl(const std::vector<uint8_t>& payload)
{
  std::cout << "Handling:";
  std::stringstream ss;
  ss << std::setfill('0');
  for (int i = 0; i < payload.size(); ++i)
  {
      ss << " 0x" << std::setw(2) << std::hex << static_cast<int>(payload[i]);
  }
  std::cout << ss.str();
  return;
}

void setup()
{
  // Set up fake data
  std::array<uint8_t,9> fakeData = {00, 01, 06, 04, 11, 22, 33, 02, 44};
  Serial.appendFakeData(fakeData);

  // Would do this in arduino
  myLspc.registerCallback(1, handl);
}

void loop()
{
  myLspc.processSerial();
}


int main(int argc, char const *argv[])
{
  setup();

  for(size_t i = 0; i < 5; i++)
  {
    loop();
  }

  return 0;
}
