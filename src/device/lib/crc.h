#ifndef CRC_H
#define CRC_H

#include <stdint.h>


class Crc {

  public:
	static uint8_t  Build8Bit(uint8_t*, uint8_t);
	static uint16_t Build16Bit(uint8_t*, uint8_t);

	static bool Validate8Bit(uint8_t*, uint8_t, uint8_t);
	static bool Validate16Bit(uint8_t*, uint8_t, uint8_t, uint8_t);
};

#endif

