#include "search.h"
#include "bus.h"
#include <vector>
#include <stdio.h>

#define MAX_DEVICES 		100
#define W1_SEARCH_ALL	 	0xF0
#define W1_SEARCH_ALARM	 	0xEC


Search::Search(Bus* bus)
	:searchBus(bus)
{};



std::vector<uint64_t> Search::Execute(bool searchAlarmOnly){

	prevDeviceId = 0;
	deviceCount  = 0;
	descBit 	 = 64;
	lastZero 	 = -1;
	isLastDevice = false;
	alarmOnly    = searchAlarmOnly;

	//in case of 2nd search
	deviceIds.clear();

	while (NotFinished() && ResetWithDevicesPresent()){
		deviceId = 0;
		DiscoverNextDevice();
		prevDeviceId = deviceId;
	}

	return deviceIds;

}


void Search::DiscoverNextDevice(void){

	int searchBit;
	uint8_t tripletRet;
	uint64_t tmp64;

	searchBus->WriteByte(alarmOnly ? W1_SEARCH_ALARM : W1_SEARCH_ALL);

	for (int bit = 0; bit < 64; ++bit) {

		searchBit  = DetermineSearchBit(bit);
		tripletRet = searchBus->Triplet(searchBit);

		/* Quit if no device responded */
		if (AnyDeviceResponds(tripletRet) == false)
			break;

		/* If both directions were valid, and we took the 0 path... */
		if (tripletRet == 0)
			lastZero = bit;

		/* Build next part of the deviceId */
		tmp64 = (tripletRet >> 2);
		deviceId |= (tmp64 << bit);
	}


	if (AnyDeviceResponds(tripletRet)){
		isLastDevice = (descBit == lastZero) || (lastZero < 0);
		descBit = lastZero;
		deviceIds.push_back(deviceId);
	}

}


int Search::DetermineSearchBit(int bit){
	if 		(bit == descBit) return 1;
	else if (bit >  descBit) return 0;
	else return ((prevDeviceId >> bit) & 0x1);
}


bool Search::AnyDeviceResponds(uint8_t tripletRet){
	return (tripletRet & 0x03) != 0x03;
}


bool Search::NotFinished(void){
	return (!isLastDevice && (deviceCount++ < MAX_DEVICES));
}


bool Search::ResetWithDevicesPresent(void){
	return searchBus->Reset() == 0;
}

