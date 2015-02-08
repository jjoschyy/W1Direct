#ifndef SEARCH_H
#define SEARCH_H

#include "bus.h"
#include <stdint.h>


class Search {

  public:
	Search(Bus*);
	std::vector<uint64_t> Execute(bool);


  private:
	void DiscoverNextDevice(void);
	int  DetermineSearchBit(int);
	bool AnyDeviceResponds(uint8_t);
	bool NotFinished(void);
	bool ResetWithDevicesPresent(void);

	Bus* searchBus;
	bool alarmOnly;
	int  deviceCount;
	int  lastZero;
	int  tripletRet;
	int  descBit;
	bool isLastDevice;

	uint64_t deviceId;
	uint64_t prevDeviceId;
	std::vector<uint64_t> deviceIds;

};

#endif
