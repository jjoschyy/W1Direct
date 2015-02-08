#include "match.h"
#include <string>
#include <stdint.h>
#include <algorithm>
#include <cstring>


static pattern patterns[] = {
  {MP_HEX_BYTE,   Match::HexByte},
  {MP_PORT_VALUE, Match::PortValue}
};


bool Match::PatternOrList(const char* matcher, const char* value){

	if (matcher == (const char*) MP_ALL_VALUES)
		return true;

	int  idx = GetPatternIdx(matcher);
	return (idx >= 0) ? patterns[idx].fn(value) : List(matcher, value);

}


bool Match::List(const char* allowedValues, const char* value){
	std::string arrayParam = std::string(value);
	std::string allowed    = std::string(allowedValues);
    std::size_t position   = allowed.find(arrayParam);

	return
	  (position != std::string::npos) &&
	  (position == 0 || allowed.at(position-1)  == '|') &&
	  (position+arrayParam.length() == allowed.length() || allowed.at(position+arrayParam.length()) == '|');

}


bool Match::HexByte(const char* value){

	return
	  strlen(value) == 4 &&
	  value[0] == '0' &&
	  value[1] == 'x' &&
	((value[2] >= 48  && value[2] <= 57) || (value[2] >= 97  &&  value[2] <= 102)) &&
	((value[3] >= 48  && value[3] <= 57) || (value[3] >= 97  &&  value[3] <= 102));

}


bool Match::PortValue(const char* value){

	return
	  strlen(value) == 4 &&
	  value[0] == 'p'  &&
	 (value[1] >= 48   && value[1] <= 55) &&
	 (value[2] == ',') &&
	 (value[3] == '0' || value[3] == '1');
}


//private


int Match::GetPatternIdx(const char* matcher){
  for (int i = 0; i < 2; i++){
	if (matcher == patterns[i].name)
	  return i;
  }

  return -1;
}

