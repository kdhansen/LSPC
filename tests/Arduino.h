#if !defined(FAKE_ARDUINO)
#define FAKE_ARDUINO

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include <queue>

class FakeSerial
{
  std::queue<uint8_t> input_buffer_;

public:
  template <size_t N>
  void AppendFakeData(std::array<uint8_t, N> data)
  {
    for(size_t i = 0; i < N; i++)
    {
      input_buffer_.push(data[i]);
    }
  }

  size_t write(unsigned char* s, size_t N)
  {
    std::stringstream ss;
    ss << std::setfill('0');
    for (int i = 0; i < N; ++i)
    {
        // ss << " " << std::setw(2) << std::hex << 55;
        ss << " 0x" << std::setw(2) << std::hex << static_cast<int>(s[i]);
    }
    std::cout << ss.str();
    return N;
  }

  size_t write(char* s, size_t N)
  {
    std::string str;
    for(size_t i = 0; i < N; i++)
    {
      str.append(s[i], N);
    }
    std::cout << str;
    return N;
  }

  size_t available()
  {
    return input_buffer_.size();
  }

  uint8_t read()
  {
    if (input_buffer_.size() == 0)
    {
      std::cerr << "Read empty buffer.";
      return 0x00;
    }
    uint8_t next_byte = input_buffer_.front();
    input_buffer_.pop();
    return next_byte;
  }
};

extern FakeSerial Serial;

#endif // FAKE_ARDUINO