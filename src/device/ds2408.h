#ifndef DS2408_H
#define DS2408_H

#include "device.h"
#include "stdint.h"
#include "../master/bus/bus.h"
#include <string>

using namespace v8;


class Ds2408 : public Device {

  public:
	Ds2408(Bus*, uint64_t, std::string*);

	void ReadAllData(void);
	bool VerifyAllData(void);
	void BuildValueData(Handle<Object>);
	void BuildPropertyData(Handle<Object>);
	bool UpdateRstzPinMode(const char*);
	bool UpdatePioOutput(const char*);
	bool UpdatePioOutputPort(const char*);
	bool UpdatePioActivity(const char*);
	bool LcdInit(const char*);
	bool LcdNewText(const char*);
	bool LcdUpdateText(const char*);


  private:
	bool PiooWriteByte(uint8_t);
	void PiooWriteMltbStart(void);
	void PiooWriteMltb(uint8_t);
	bool PiooWriteMltbSucceed(void);

	void LcdWriteByte(uint8_t, uint8_t);
	void LcdClear(void);
	void LcdSetCursor(uint8_t, uint8_t);
	void LcdDisplayText(const char*);

	void BuildValue(Handle<Object>, const char*, int);

};

#endif
