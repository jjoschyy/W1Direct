#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <stdint.h>


class Util {

  public:
    static void ThrowExceptionIf(bool, const char*, ...);
    static std::string UInt64ToHexStr(uint64_t);
	static int StrPartToInt(const char*, int);

    static void SetBit(uint8_t*, uint8_t, bool);
    static bool BitIsSet(int, int);
    static bool BitIsMasked(int, int);
	static void AddToBitMaskIf(int*, std::string*, const char*, int);
};

#endif

