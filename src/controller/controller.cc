#include "controller.h"
#include "device_store.h"
#include "../master/master.h"
#include "../device/ds18b20.h"
#include "../device/ds18s20.h"
#include "../device/ds2408.h"
#include "../device/unsupported.h"
#include "../shared/util.h"
#include <vector>
#include <stdint.h>
#include <string>



Controller::Controller(void) {
	deviceStore = new DeviceStore();
	syncId = 0;
}


void Controller::AddMaster(std::string* name, Master* master) {
	masters[*name] = master;
}


Master* Controller::GetMaster(std::string* name){
	return masters[*name];
}


bool Controller::HasMaster(std::string* name){
	return masters.find(*name) != masters.end();
}


void Controller::SyncAllDevices(void) {
	std::map<std::string, Master*>::iterator itM;

	for (itM = masters.begin(); itM != masters.end(); ++itM)
		SyncMasterDevices(itM->second);

}


void Controller::SyncMasterDevices(Master* master) {
	std::vector<Bus*>::iterator itB;
	std::vector<Bus*> *buses = master->GetBuses();

	for (itB = buses->begin(); itB !=buses->end(); ++itB)
		SyncBusDevices(*itB);

}


void Controller::SyncBusDevices(Bus* bus) {

	//overdrive must be disabled on sync
	bus->SetOverdriveSpeed(false);

	std::vector<uint64_t> deviceIds = bus->SearchDeviceIds(false);
	std::vector<uint64_t>::iterator it;
	syncId+=1;

	for(it = deviceIds.begin(); it != deviceIds.end(); it++)
		SyncFoundBusDevice(bus, *it);

	deviceStore->RemoveBusDeviceIf(bus, &syncId, CheckDeviceDeletion);
}


DeviceStore* Controller::GetDeviceStore(void){
	return deviceStore;
}


void Controller::SyncFoundBusDevice(Bus* bus, uint64_t intDeviceId){

	Device* device;
	std::string strDeviceId = Util::UInt64ToHexStr(intDeviceId);

	if (!deviceStore->HasDevice(&strDeviceId)){
		device = NewDevice(bus, intDeviceId, &strDeviceId);
		deviceStore->AddDevice(&strDeviceId, device);
		device->AfterNewSearched(syncId);

	} else {
		device = deviceStore->GetDevice(&strDeviceId);
		deviceStore->AddChange(CHG_UPDATED, device);
		device->AfterAgainSearched(syncId);
	}

}


bool Controller::CheckDeviceDeletion(Device* busDevice, void* args){
	return busDevice->GetSyncId() != *((uint64_t*) args);
}



Device* Controller::NewDevice(Bus* bus, uint64_t intDeviceId, std::string* strDeviceId){

	uint8_t familyCode = (uint8_t) (intDeviceId);

	switch (familyCode){
		case 0x10 : return new Ds18s20(bus, intDeviceId, strDeviceId);
		case 0x28 : return new Ds18b20(bus, intDeviceId, strDeviceId);
		case 0x29 : return new Ds2408(bus, intDeviceId, strDeviceId);
		default	  : return new Unsupported(bus,  intDeviceId, strDeviceId);
	}
}
