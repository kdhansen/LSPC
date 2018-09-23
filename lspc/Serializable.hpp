#ifndef LSPC_SERIALIZABLE_HPP
#define LSPC_SERIALIZABLE_HPP

#include <vector>
#include <cstdint>

namespace lspc
{

class Serializable
{
public:
  virtual std::vector<uint8_t> serialize() = 0;
  virtual uint8_t type() = 0;
};

} // namespace lspc

#endif // LSPC_SERIALIZABLE_HPP