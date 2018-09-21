#include "lspc/arduino/Socket.hpp"

#include <array>
#include <cstdint>
#include <iostream>

#include "Arduino.h"

FakeSerial Serial;
lspc::Socket myLspc;

void setup()
{}

void loop()
{
  std::vector<uint8_t> payload = {11, 22, 33, 0, 44};
  myLspc.send(1, payload);
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
