#include "device_store.h"
#include "../device/device.h"
#include "../master/bus/bus.h"
#include <string>


Device* DeviceStore::GetDevice(std::string* strDeviceId){
	return devices[*strDeviceId];
}


bool DeviceStore::HasDevice(std::string* deviceId){
	return devices.find(*deviceId) != devices.end();
}



void DeviceStore::AddDevice(std::string* strDeviceId, Device* device){
	devices[*strDeviceId] = device;
	AddChange(CHG_ADDED, device);
}


void DeviceStore::RemoveDevice(std::string* strDeviceId){
	Device* device = GetDevice(strDeviceId);
	devices.erase(*strDeviceId);
	AddChange(CHG_REMOVED, device);
}


void DeviceStore::RemoveBusDeviceIf(Bus* bus, void* opts, bool (*cb)(Device*, void* opts)){

	std::map<std::string, Device*>::iterator it;
	bool match, delDev;
	Device *device;

	for(it = devices.begin(); it != devices.end(); it++){
		device = it->second;
		match  = device->GetBus() == bus;
		delDev = match && cb(it->second, opts);
		if (delDev) RemoveDevice(device->GetStrId());
	}

}


void DeviceStore::AddChange(const int type, Device* device){
	changes[type].push_back(device);
}


std::vector<Device*>* DeviceStore::GetChanges(int type){
	return &(changes[type]);
}


void DeviceStore::ResetAllChanges(bool deleteRemovedDevices){

	if (deleteRemovedDevices){
		std::vector<Device*>::iterator it;
		for(it = changes[CHG_REMOVED].begin(); it != changes[CHG_REMOVED].end(); it++)
			delete *it;
	}

	changes[CHG_ADDED].clear();
	changes[CHG_UPDATED].clear();
	changes[CHG_REMOVED].clear();
}





