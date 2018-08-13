# Lightweight Serial Package Communication
This library aims help utilise the bandwidth better when communicating between a
Teensy and a PC over the serial connection.

It packages a serialized message with type and lenght descriptors and uses
consistent overhead byte stuffing (COBS) to allow using 0x00 as package header.
It is the responsibility of you to write the serialization. The library make
sure that the serialized message is safely transfered over the link. On the
other end, the library provides the functions to decode the package into a
serialized message, that you can deserialize however you want.
