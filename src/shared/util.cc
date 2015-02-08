#include "util.h"
#include <node.h>
#include <stdarg.h> 
#include <string>
#include <stdint.h>
#include <algorithm>
#include <math.h>

#define MAX_EXCEPTION_MSG_LEN 100

using namespace v8;


void Util::ThrowExceptionIf(bool isException, const char* format, ...){

	if (isException){
		char msg[MAX_EXCEPTION_MSG_LEN+1];
		va_list args;
		va_start(args, format);
		vsnprintf(msg, MAX_EXCEPTION_MSG_LEN, format, args);
		va_end(args);
		ThrowException(Exception::Error(String::New(msg)));
	}

}


std::string Util::UInt64ToHexStr(uint64_t value){

	uint8_t byte;
	char buffer[17];

	for (int i = 0; i < 8; i++){
	   byte = (uint8_t) (value >> i*8);
	   sprintf(&buffer[i*2], "%02X", byte);
	}

	return std::string(buffer);

}


int Util::StrPartToInt(const char* str, int charCount){

	int value = 0;

	for (int i = charCount; i > 0; i--)
		value += ((str[i-1] - '0') * (int) pow(10, i-1));

	return value;
}


void Util::SetBit(uint8_t* byte, uint8_t bitNo, bool setBitToOne){
	setBitToOne ? (*byte |= 1 << bitNo) : (*byte &= ~(1 << bitNo));
}


bool Util::BitIsSet(int value, int bitNo){
	return (((value) >> (bitNo)) & 1);
}


bool Util::BitIsMasked(int mask, int maskConst){
	 return ((mask & maskConst) == maskConst);
}


void Util::AddToBitMaskIf(int *mask, std::string *value, const char* match, int maskValue){
	if (value->compare(match) == 0)
		*mask += maskValue;
}
