#ifndef MASTER_H
#define MASTER_H

#include "bus/bus.h"
#include <vector>
#include <stdint.h>
#include <string>

class Master {

  public:
	Master(std::string*);

	virtual void 	W1WriteByte(uint8_t) 	= 0;
	virtual uint8_t W1ReadByte(void)	 	= 0;
	virtual uint8_t W1Triplet(uint8_t)   	= 0;
	virtual uint8_t W1Reset(void)        	= 0;

	virtual void 	SelectBus(Bus*)    	 	= 0;
	virtual void 	SetOverdriveSpeed(bool) = 0;

	std::string* GetName(void);

	bool HasBus(unsigned int);
	Bus* GetBus(unsigned int);
	std::vector<Bus*>* GetBuses(void);

	void SetSelectedBus(Bus*);
	Bus* GetSelectedBus(void);

	
  protected:
	void AddBus();

  private:
	std::vector<Bus*> buses;
	std::string name;
	Bus* selectedBus;

};

#endif
