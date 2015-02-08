#ifndef DS18B20_H
#define DS18B20_H

#include "device.h"
#include "stdint.h"
#include "../master/bus/bus.h"
#include <string>

using namespace v8;


class Ds18b20 : public Device {

  public:
	Ds18b20(Bus*, uint64_t, std::string*);

	bool Initialize(void);
	void ReadValueData(void);
	void ReadPropertyData(void);
	bool VerifyValueData(void);
	void BuildValueData(Handle<Object>);
	void BuildPropertyData(Handle<Object>);
	bool UpdateResolution(const char*);


  private:
	void BuildTCelsius(Handle<Object>, const char*);
	void WriteConfigRegister(uint8_t);
	bool VerifyConfigRegister(uint8_t);


};

#endif
