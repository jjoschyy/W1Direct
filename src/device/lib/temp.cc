#include "temp.h"
#include <stdint.h>


typedef struct {
  const char* r9bit;
  const char* r10bit;
  const char* r11bit;
  const char* r12bit;
} decimal;


static decimal DECIMALS[] = {
  {"0", "0",  "0",   "0"   },
  {"0", "0",  "0",   "0625"},
  {"0", "0",  "125", "125" },
  {"0", "0",  "125", "1875"},
  {"0", "25", "25",  "25"  },
  {"0", "25", "25",  "3125"},
  {"0", "25", "375", "375" },
  {"0", "25", "375", "4375"},
  {"5", "5",  "5",   "5"   },
  {"5", "5",  "5",   "5625"},
  {"5", "5",  "625", "625" },
  {"5", "5",  "625", "6875"},
  {"5", "75", "75",  "75"  },
  {"5", "75", "75",  "8125"},
  {"5", "75", "875", "875" },
  {"5", "75", "875", "9375"}
};

#define DECIMAL_NOT_FOUND "?"



uint16_t Temp::ConcatMsbLsb(uint8_t msb, uint8_t lsb){
	uint16_t meas = lsb;

	meas |= ((uint16_t) msb) << 8;
	return meas;
}


void Temp::HandleSubzero(uint16_t *meas, uint8_t *subzero){

  //check for negative
  if (*meas & 0x8000)  {

	//convert to positive (twos complement)++
	*meas ^= 0xffff;
	*meas +=1;
	*subzero = true;

  } else
    //no action required
	*subzero = false;

}


const char* Temp::DecimalsToString(uint8_t bitResolution, uint8_t decimalByte){
	decimal dec = DECIMALS[decimalByte];

	switch(bitResolution) {
	  case 9:  return dec.r9bit;
	  case 10: return dec.r10bit;
	  case 11: return dec.r11bit;
	  case 12: return dec.r12bit;
	}

	return (char*) DECIMAL_NOT_FOUND;  //should never happen
}



uint8_t Temp::ResolutionFromString(const char* resolution){

	//extract number from 9bit, 10bit, 11bit, 12bit
	return resolution[0] == '9' ? 9 : resolution[1]-38;
}
