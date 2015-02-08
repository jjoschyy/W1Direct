#ifndef API_H
#define API_H

#include "../controller/controller.h"
#include "../shared/v8_helper.h"
#include <node.h>
#include <vector>
#include <string>

#define DEVICE_VECTOR std::vector<Device*>

#define DP_MASTER_NAME "masterName"
#define DP_BUS_NUMBER  "busNumber"
#define DP_DEVICE_ID   "deviceId"
#define DP_DEVICE_IDS  "deviceIds"


using namespace v8;


class Api : public V8Helper {

protected:
   static bool  	  	 AssertParamsFormat(const Arguments&);
   static bool  	  	 AssertParam(const Arguments&, const char*, const char*);
   static bool  	  	 AssertParamIn(const Arguments&, const char*, const char*);
   static bool  	  	 AssertArrayParamIn(const Arguments&, const char*, const char*);
   static bool 		  	 AssertDefaultParam(const Arguments&, const char*);

   static bool 		  	 AssertMaster(const Arguments&);
   static bool 		  	 AssertBus(const Arguments&);
   static bool 		  	 AssertDevice(const Arguments&);
   static bool 		  	 AssertDevice(const Arguments&, std::string*);
   static bool 		  	 AssertDevices(const Arguments&);

   static Controller* 	 GetController(const Arguments&);
   static Master*     	 GetMaster(const Arguments&);
   static Bus*        	 GetBus(const Arguments&);
   static Device* 		 GetDevice(const Arguments&);
   static DEVICE_VECTOR  GetDevices(const Arguments&);

   static std::string 	 GetStrParam(const Arguments&, const char*);
   static int 		  	 GetIntParam(const Arguments&, const char*);
   static Handle<Array>	 GetV8ArrayParam(const Arguments&s, const char*);

   static Handle<Array>  DevicesToV8Array(std::vector<Device*>*,  int);
   static Handle<Object> DevicesToV8Object(std::vector<Device*>*, int);

};


#endif
