#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "device_store.h"
#include "../master/master.h"
#include "../device/device.h"
#include <map>
#include <string>
#include <stdint.h>


class Controller {

  public:
	Controller(void);
    void AddMaster(std::string*, Master*);
    Master* GetMaster(std::string*);
    bool HasMaster(std::string*);

    void SyncAllDevices(void);
    void SyncMasterDevices(Master*);
   	void SyncBusDevices(Bus*);

   	DeviceStore* GetDeviceStore(void);
   	std::map<std::string, Master*> masters;


  private:
   	void SyncFoundBusDevice(Bus*, uint64_t);
   	Device* NewDevice(Bus*, uint64_t, std::string*);
   	static bool CheckDeviceDeletion(Device*, void*);

    DeviceStore* deviceStore;
    uint64_t syncId;


};

#endif
