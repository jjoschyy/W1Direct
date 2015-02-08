#include "update.h"
#include "../shared/util.h"
#include <vector>
#include <algorithm>

using namespace v8;

#define CP_SET 	  "set"
#define CP_VALUE  "value"


Handle<Value> Update::DeviceById(const Arguments& args) {
	HandleScope scope;

	bool validArgs =
	  AssertParamsFormat(args) 				 &&
  	  AssertDefaultParam(args, DP_DEVICE_ID) &&
  	  AssertParam(args, CP_SET,   DT_STRING) &&
  	  AssertParam(args, CP_VALUE, DT_STRING) &&
  	  AssertDevice(args)					 &&
  	  AssertUpdaterExists(args)				 &&
  	  AssertUpdaterValue(args);


	if (validArgs){
		Handle<Object> result = Object::New();
		AddPairToV8Object(result, "crcError", !ExecuteUpdater(args));
		return scope.Close(result);
	}

	return scope.Close(Undefined());
}



// private

bool Update::AssertUpdaterExists(const Arguments& args){
	Device* device = GetDevice(args);
	std::string name = GetUpdaterName(args);
	bool supported = device->SupportsUpdater(&name);

	Util::ThrowExceptionIf(!supported, "Update of '%s' is not supported on device %s", name.c_str(), device->GetStrId()->c_str());
	return supported;
}


bool Update::AssertUpdaterValue(const Arguments& args){
	std::string updaterName = GetUpdaterName(args);
	return AssertParamIn(args, CP_VALUE, GetDevice(args)->GetUpdaterValidator(&updaterName));
}


std::string Update::GetUpdaterName(const Arguments& args){
	return GetStrParam(args, CP_SET);
}


bool Update::ExecuteUpdater(const Arguments& args){
	std::string name =  GetUpdaterName(args);
	std::string value = GetStrParam(args, CP_VALUE);

	return GetDevice(args)->ExecuteUpdater(&name, &value);
}
