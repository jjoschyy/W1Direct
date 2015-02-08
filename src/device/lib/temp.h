#ifndef TEMP_H
#define TEMP_H

#include <stdint.h>

class Temp {

  public:
	static uint16_t   	ConcatMsbLsb(uint8_t, uint8_t);
	static void  	  	HandleSubzero(uint16_t*, uint8_t*);
	static const char*  DecimalsToString(uint8_t, uint8_t);
	static uint8_t 	  	ResolutionFromString(const char*);

};


#endif
