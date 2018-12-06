#include "lspc/arduino/Socket.hpp"

#include <array>
#include <cstdint>
#include <iostream>

#include "Arduino.h"

FakeSerial Serial;
lspc::Socket myLspc;

class Type1Object : public lspc::Serializable
{
  std::vector<uint8_t> payload_ = {55, 66, 77, 0, 88};
  uint8_t type_ = 1;

public:
  std::vector<uint8_t> serialize() const override
  {
    return payload_;
  }

  uint8_t type() const override
  {
    return type_;
  }
};

void setup()
{
  std::vector<uint8_t> payload = {11, 22, 33, 0, 44};
  myLspc.send(1, payload);

  Type1Object obj;
  myLspc.send(obj);
}

void loop()
{}


int main(int argc, char const *argv[])
{
  setup();

  for(size_t i = 0; i < 5; i++)
  {
    loop();
  }

  return 0;
}
