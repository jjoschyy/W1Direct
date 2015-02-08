#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <stdint.h>

#define MP_ALL_VALUES ""
#define MP_HEX_BYTE   "{0x??}"
#define MP_PORT_VALUE "{p0-7,0|1}"



typedef struct {
  const char* name;
  bool (*fn)(const char*);
} pattern;



class Match {

  public:
	static bool PatternOrList(const char*, const char*);
	static bool List(const char*, const char*);
	static bool HexByte(const char*);
	static bool PortValue(const char*);


  private:
	static int GetPatternIdx(const char*);
};

#endif





