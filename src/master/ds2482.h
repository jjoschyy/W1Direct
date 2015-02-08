#ifndef DS2482_H
#define DS2482_H

#include "master.h"
#include <string>


class DS2482: public Master {

  public:
	DS2482(std::string*, std::string*);
	bool Initialize(std::string*, int);
	
	virtual uint8_t W1Reset(void);
	virtual void	W1WriteByte(uint8_t);
	virtual uint8_t W1ReadByte(void);
	virtual uint8_t W1Triplet(uint8_t);

	virtual void 	SelectBus(Bus*);
	virtual void 	SetOverdriveSpeed(bool);
	

  private:
	uint8_t ReadRegUntilW1Idle(void);
	uint8_t ReadByte(void);
	uint8_t CalculateConfig(uint8_t);
	bool    SelectRegister(uint8_t);
	bool    SendCmd(uint8_t);
	bool    SendCmdWithData(uint8_t, uint8_t);

	int masterFd;
};


#endif
