#ifndef DEVICE_STORE_H
#define DEVICE_STORE_H

#include "../device/device.h"
#include "../master/bus/bus.h"
#include <map>
#include <string>
#include <vector>

#define CHG_ADDED   0
#define CHG_UPDATED 1
#define CHG_REMOVED 2


class DeviceStore {

  public:
	Device* GetDevice(std::string*);
	void AddDevice(std::string*, Device*);
	bool HasDevice(std::string*);
	void RemoveDevice(std::string*);
	void RemoveBusDeviceIf(Bus*, void*, bool (*cb)(Device*, void* opts));

	void AddChange(const int, Device*);
	std::vector<Device*>* GetChanges(int);
	void ResetAllChanges(bool);


  private:
	std::map<std::string, Device*> devices;
	std::vector<Device*> changes[3];

};


#endif







