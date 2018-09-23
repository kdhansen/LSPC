find_package(Boost REQUIRED system)

include(CTest)

include_directories(.)
include_directories(${Boost_INCLUDE_DIRS})


###### Packet: Encoding Tests ######
add_executable(test_packet_encoding test_packet_encoding.cpp)

# Test a simple benign input. This should work.
add_test(NAME PacketEncodingBasic COMMAND test_packet_encoding 1 11 22 0 33)
set_tests_properties (PacketEncodingBasic PROPERTIES
  PASS_REGULAR_EXPRESSION "0 1 5 3 11 22 2 33")

# Test the same input but without zeros in the data. Should also work.
add_test(NAME PacketEncodingNoZeros COMMAND test_packet_encoding 1 11 22 33 44)
set_tests_properties(PacketEncodingNoZeros PROPERTIES
  PASS_REGULAR_EXPRESSION "0 1 5 5 11 22 33 44")

# Test an empty message but with a proper type. This should work.
add_test(NAME PacketEncodingEmptyPayload COMMAND test_packet_encoding 2)
set_tests_properties (PacketEncodingEmptyPayload PROPERTIES
  PASS_REGULAR_EXPRESSION "0 2 1 1")

# Test an empty message with an type of zero. This should fail.
add_test(NAME PacketEncodingZeroType COMMAND test_packet_encoding 0)
set_tests_properties (PacketEncodingZeroType PROPERTIES WILL_FAIL true)

# Test a too long payload. This should fail.
add_test(NAME PacketEncodingLongPayload COMMAND test_packet_encoding 1 1 2 3 4
  5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
  32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57
  58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83
  84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106
  107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125
  126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144
  145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163
  164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182
  183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201
  202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220
  221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239
  240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 1 2 3)
set_tests_properties (PacketEncodingLongPayload PROPERTIES WILL_FAIL true)


###### Packet: Encoding tests without exceptions (Arduino Style) ######
add_executable(test_encoding_noexeptions test_packet_encoding.cpp)
get_target_property(TEMP test_encoding_noexeptions COMPILE_FLAGS)
if(TEMP STREQUAL "TEMP-NOTFOUND")
  SET(TEMP "") # set to empty string
else()
  SET(TEMP "${TEMP} ") # a space to cleanly separate from existing content
endif()
# append our values
SET(TEMP "${TEMP}-fno-exceptions")
set_target_properties(test_encoding_noexeptions PROPERTIES COMPILE_FLAGS ${TEMP})

add_test(NAME PacketEncodingBasicNoExcept
  COMMAND test_encoding_noexeptions 1 11 22 0 33)
set_tests_properties (PacketEncodingBasicNoExcept PROPERTIES
  PASS_REGULAR_EXPRESSION "0 1 5 3 11 22 2 33")

add_test(NAME PacketEncodingZeroTypeNoExcept
  COMMAND test_encoding_noexeptions 0)
set_tests_properties (PacketEncodingZeroTypeNoExcept PROPERTIES WILL_FAIL true)


###### Packet: Decoding Tests ######
add_executable(test_packet_decoding test_packet_decoding.cpp)

# Test a benign encoded input. This should work.
add_test(NAME PacketDecoding COMMAND test_packet_decoding 0 1 5 3 11 22 2 33)
set_tests_properties (PacketDecoding PROPERTIES
  PASS_REGULAR_EXPRESSION "Payload: 11 22 0 33, Type: 1")

# Test a benign encoded input, without any payload. This should work.
add_test(NAME PacketDecodingNoPayload COMMAND test_packet_decoding 0 1 1 1)
set_tests_properties (PacketDecodingNoPayload PROPERTIES
  PASS_REGULAR_EXPRESSION "Payload:, Type: 1")

# Test a long buffer of 258 bytes. This should work.
add_test(NAME PacketDecodingLongBuffer
  COMMAND test_packet_decoding 0 1 255 255 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40
    41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65
    66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90
    91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111
    112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130
    131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149
    150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168
    169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187
    188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206
    207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225
    226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244
    245 246 247 248 249 250 251 252 253 254)
set_tests_properties (PacketDecodingLongBuffer PROPERTIES
  PASS_REGULAR_EXPRESSION "Payload: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17\
 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43\
 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69\
 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95\
 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115\
 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134\
 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153\
 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172\
 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191\
 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210\
 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229\
 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248\
 249 250 251 252 253 254, Type: 1")

# Set the length parameter to one less than the data length. This should fail.
add_test(NAME PacketDecodingShortLength
  COMMAND test_packet_decoding 0 1 4 3 11 22 2 33)
set_tests_properties (PacketDecodingShortLength PROPERTIES WILL_FAIL true)

# Set the length parameter to one more than the data length. This should fail.
add_test(NAME PacketDecodingLongLength
  COMMAND test_packet_decoding 0 1 6 3 11 22 2 33)
set_tests_properties (PacketDecodingLongLength PROPERTIES WILL_FAIL true)

# Set the final COBS offset code to point to one short. This should fail. Note
# that the decoder should interpret the "33", that the wrong code points to, as
# another code, so that code will actually point waybeyound the end of the data.
add_test(NAME PacketDecodingCobsCodeShort
  COMMAND test_packet_decoding 0 1 5 3 11 22 1 33)
set_tests_properties (PacketDecodingCobsCodeShort PROPERTIES WILL_FAIL true)

# Set the final COBS offset code to point to one beyound the data. This should
# fail. Given the explanation above, these two tests may actually test the same
# functionality. We keep it for symmetry.
add_test(NAME PacketDecodingCobsCodeLong
  COMMAND test_packet_decoding 0 1 5 3 11 22 1 33)
set_tests_properties (PacketDecodingCobsCodeLong PROPERTIES WILL_FAIL true)

# Set the type to zero. This should fail.
add_test(NAME PacketDecodingZeroType
  COMMAND test_packet_decoding 0 0 5 3 11 22 2 33)
set_tests_properties (PacketDecodingZeroType PROPERTIES WILL_FAIL true)

# Test a too long buffer. This should fail.
 add_test(NAME PacketDecodingTooLongBuffer COMMAND test_packet_encoding 0 1 255
 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55
 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81
 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105
 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125
 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145
 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165
 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185
 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205
 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225
 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245
 246 247 248 249 250 251 252 253 254 255)
set_tests_properties (PacketDecodingTooLongBuffer PROPERTIES WILL_FAIL true)

###### Packet: Output tests ######
add_executable(test_packet_output test_packet_output.cpp)
add_test(NAME PacketOutput COMMAND test_packet_output)


###### Arduino API test ######
add_executable(test_arduino_api_send test_arduino_api_send.cpp)
add_test(NAME ArduinoApiSend COMMAND test_arduino_api_send)
set_tests_properties (ArduinoApiSend PROPERTIES
  PASS_REGULAR_EXPRESSION " 0x00 0x01 0x06 0x04 0x0b 0x16 0x21 0x02 0x2c \
0x00 0x01 0x06 0x04 0x37 0x42 0x4d 0x02 0x58\n")

add_executable(test_arduino_api_receive test_arduino_api_receive.cpp)
add_test(NAME ArduinoApiReceive COMMAND test_arduino_api_receive)
set_tests_properties (ArduinoApiReceive PROPERTIES
  PASS_REGULAR_EXPRESSION "Handling: 0x0b 0x16 0x21 0x00 0x2c\n")


###### Boost API test ######
add_executable(test_boost_api_send test_boost_api_send.cpp)
target_link_libraries(test_boost_api_send ${Boost_LIBRARIES} ws2_32)
add_test(NAME BoostApiSend COMMAND test_boost_api_send COM8 COM9)
set_tests_properties (BoostApiSend PROPERTIES
  PASS_REGULAR_EXPRESSION " 0 1 6 4 11 22 33 2 44\n 0 1 6 4 55 66 77 2 88")

add_executable(test_boost_api_receive test_boost_api_receive.cpp)
target_link_libraries(test_boost_api_receive ${Boost_LIBRARIES} ws2_32)
add_test(NAME BoostApiReceive COMMAND test_boost_api_receive COM8 COM9)
set_tests_properties (BoostApiReceive PROPERTIES
  PASS_REGULAR_EXPRESSION "Handling: 0x0b 0x16 0x21 0x00 0x2c\n")
set_tests_properties (BoostApiReceive PROPERTIES
  DEPENDS BoostApiSend)
