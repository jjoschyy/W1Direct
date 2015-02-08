#include "bus.h"
#include "search.h"
#include "../ds2482.h"
#include <stdint.h>
#include <vector>


#define W1_MATCH_ROM    		  0x55
#define W1_MATCH_ROM_OVERDRIVE    0x69
#define W1_SKIP_ROM     		  0xCC
#define W1_SKIP_ROM_OVERDRIVE     0x3C


Bus::Bus(Master* master, int number)
	: master(master), number(number), overdriveSpeed(false)
{}


Master* Bus::GetMaster(void){
	return master;
}


int Bus::GetNumber(void){
	return number;
}


std::vector<uint64_t> Bus::SearchDeviceIds(bool alarmOnly){
	Search *search = new Search(this);
	std::vector<uint64_t> deviceIds = search->Execute(alarmOnly);
	delete search;

	return deviceIds;
}


void Bus::DeviceCommand(uint64_t deviceId, uint8_t command){
	
	Reset();
	WriteByte(W1_MATCH_ROM);

	for (uint8_t i = 0; i < 8; i++)
	  WriteByte((uint8_t) (deviceId >> i*8));

	WriteByte(command);
}


void Bus::BroadcastCommand(uint8_t command){
	Reset();
	WriteByte(W1_SKIP_ROM);
	WriteByte(command);
}


void Bus::SetOverdriveSpeed(bool useOverdriveSpeed){

	//1. inform devices about overdrive
	if (!overdriveSpeed && useOverdriveSpeed){
	     Reset();
	     WriteByte(W1_SKIP_ROM_OVERDRIVE);
	}

	//2. inform master about bus speed
	master->SetOverdriveSpeed(useOverdriveSpeed);
	overdriveSpeed = useOverdriveSpeed;
}


uint8_t Bus::Reset(void){
	Select();
	return master->W1Reset();
}


void Bus::WriteByte(uint8_t byte){
	Select();
	master->W1WriteByte(byte);
}

uint8_t Bus::ReadByte(void){
	Select();
	return master->W1ReadByte();
}


uint8_t Bus::Triplet(uint8_t bDir){
	Select();
	return master->W1Triplet(bDir);
}


void Bus::Select(void){
	
	if (master->GetSelectedBus() != this){
		master->SelectBus(this);
		master->SetSelectedBus(this);
	}
}


