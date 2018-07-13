include(CTest)

include_directories(.)

add_executable(test_packet_encoding test_packet_encoding.cpp)

add_test(NAME PacketEncodingBasic COMMAND test_packet_encoding 1 11 22 0 33)
set_tests_properties (PacketEncodingBasic PROPERTIES PASS_REGULAR_EXPRESSION "0 1 5 3 11 22 2 33")

add_test(NAME PacketEncodingNoZeros COMMAND test_packet_encoding 1 11 22 33 44)
set_tests_properties (PacketEncodingNoZeros PROPERTIES PASS_REGULAR_EXPRESSION "0 1 5 5 11 22 33 44")

add_test(NAME PacketEncodingEmptyPayload COMMAND test_packet_encoding 2)
set_tests_properties (PacketEncodingEmptyPayload PROPERTIES PASS_REGULAR_EXPRESSION "0 2 1 1")

add_test(NAME PacketEncodingZeroType COMMAND test_packet_encoding 0)
set_tests_properties (PacketEncodingZeroType PROPERTIES WILL_FAIL true)


add_executable(test_packet_decoding test_packet_decoding.cpp)

add_test(NAME PacketDecoding COMMAND test_packet_decoding 0 1 5 3 11 22 2 33)
set_tests_properties (PacketDecoding PROPERTIES PASS_REGULAR_EXPRESSION "Payload: 11 22 0 33, Type: 1")


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

add_test(NAME PacketEncodingBasicNoExcept COMMAND test_encoding_noexeptions 1 11 22 0 33)
set_tests_properties (PacketEncodingBasicNoExcept PROPERTIES PASS_REGULAR_EXPRESSION "0 1 5 3 11 22 2 33")

add_test(NAME PacketEncodingZeroTypeNoExcept COMMAND test_encoding_noexeptions 0)
set_tests_properties (PacketEncodingZeroTypeNoExcept PROPERTIES WILL_FAIL true)