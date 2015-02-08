#include "api.h"
#include "../manager.h"
#include "../shared/util.h"
#include "../shared/match.h"
#include "../controller/controller.h"
#include <node.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace v8;


bool Api::AssertParamsFormat(const Arguments& args){
	bool valid = args.Length() > 0 && V8ValueIsFromDataType(args[0], DT_OBJECT);
	Util::ThrowExceptionIf(!valid, "First argument must be from data type 'object'");

	return valid;
}


bool Api::AssertParam(const Arguments& args, const char* key, const char* dataType) {
	bool paramExists  = V8ObjectHasKey(args[0], key);
	bool paramCorrect = paramExists && V8ValueIsFromDataType(GetV8ValueFromV8Object(args[0], key), dataType);

	Util::ThrowExceptionIf(!paramExists, "Param missing: %s", key);
	Util::ThrowExceptionIf(paramExists && !paramCorrect, "Data type for param '%s' must be '%s'", key, dataType);

	return paramCorrect;
}


bool Api::AssertParamIn(const Arguments& args, const char* key, const char* matcher){
	std::string value = GetStrParam(args, key);

	bool match = Match::PatternOrList(matcher, value.c_str());
	Util::ThrowExceptionIf(!match, "Value '%s' invalid for param '%s'. Allowed values: %s", value.c_str(), key, matcher);

	return match;
}


bool Api::AssertArrayParamIn(const Arguments& args, const char* key, const char* matcher){

	Handle<Array> arrayParams = GetV8ArrayParam(args, key);
	std::string arrayParam; bool match;

	for (unsigned int i=0 ; i < arrayParams->Length(); ++i){
	    arrayParam = V8ValueToStdString(arrayParams->Get(i));
	    match = Match::PatternOrList(matcher, arrayParam.c_str());
	    Util::ThrowExceptionIf(!match, "Value '%s' invalid for array '%s'. Allowed values: %s", arrayParam.c_str(), key, matcher);

	    if (!match)
	    	return false;
	}

	return true;
}


bool Api::AssertDefaultParam(const Arguments& args, const char* param){

	if (param == (const char*) DP_MASTER_NAME)
		return AssertParam(args, param, DT_STRING);

	if (param == (const char*) DP_BUS_NUMBER)
		return AssertParam(args, param, DT_NUMBER);

	if (param == (const char*) DP_DEVICE_ID)
		return AssertParam(args, param, DT_STRING);

	if (param == (const char*) DP_DEVICE_IDS)
		return AssertParam(args, param, DT_ARRAY);

	return false;
}



bool Api::AssertMaster(const Arguments& args){
	Controller *ctl = GetController(args);
	std::string name = GetStrParam(args, DP_MASTER_NAME);
	bool masterFound = ctl->HasMaster(&name);
	Util::ThrowExceptionIf(!masterFound, "The master '%s' has not been registered.", name.c_str());

	return masterFound;
}


bool Api::AssertBus(const Arguments& args){
	int busNumber = GetIntParam(args, DP_BUS_NUMBER);
	bool busFound = GetMaster(args)->HasBus(busNumber);
	Util::ThrowExceptionIf(!busFound, "The bus '%d' on master '%s' does not exist.", busNumber, GetMaster(args)->GetName()->c_str());

	return busFound;
}


bool Api::AssertDevice(const Arguments& args){
	std::string deviceId = GetStrParam(args, DP_DEVICE_ID);
	return AssertDevice(args, &deviceId);
}


bool Api::AssertDevice(const Arguments& args, std::string* deviceId){
	Controller *ctl = GetController(args);

	bool deviceExists = ctl->GetDeviceStore()->HasDevice(deviceId);
	bool deviceReady  = deviceExists && ctl->GetDeviceStore()->GetDevice(deviceId)->IsReady();

	Util::ThrowExceptionIf(!deviceExists,  "Device '%s' does not exist.", deviceId->c_str());
	Util::ThrowExceptionIf(deviceExists && !deviceReady, "Device '%s' is not in READY state.", deviceId->c_str());

	return deviceReady;
}


bool Api::AssertDevices(const Arguments& args){

	std::string deviceId;
	Handle<Array> deviceIds = GetV8ArrayParam(args, DP_DEVICE_IDS);

	for (unsigned int i=0 ; i < deviceIds->Length(); ++i){
	  deviceId = V8ValueToStdString(deviceIds->Get(i));
	  if (!AssertDevice(args, &deviceId)) return false;
	}

	return true;
}


Controller* Api::GetController(const Arguments& args){
	return node::ObjectWrap::Unwrap<Manager>(args.This())->controller;
}


Master* Api::GetMaster(const Arguments& args){
	std::string masterName = GetStrParam(args, DP_MASTER_NAME);
	return GetController(args)->GetMaster(&masterName);
}


Bus* Api::GetBus(const Arguments& args){
	int busNumber = GetIntParam(args, DP_BUS_NUMBER);
	return GetMaster(args)->GetBus(busNumber);
}


Device* Api::GetDevice(const Arguments& args){
	std::string deviceId = GetStrParam(args, DP_DEVICE_ID);
	return GetController(args)->GetDeviceStore()->GetDevice(&deviceId);
}


std::vector<Device*> Api::GetDevices(const Arguments& args){

	std::string deviceId;
	std::vector<Device*> devices;

	Controller *ctl = GetController(args);
	Handle<Array> deviceIds = GetV8ArrayParam(args, DP_DEVICE_IDS);

	for (unsigned int i=0; i<deviceIds->Length(); ++i){
		deviceId = V8ValueToStdString(deviceIds->Get(i));
		devices.push_back(ctl->GetDeviceStore()->GetDevice(&deviceId));
	}

	//Sort by "overdrive->master->bus" for efficient read/write
	std::sort(devices.begin(), devices.end());

	return devices;
}


std::string Api::GetStrParam(const Arguments& args, const char* name){
	return GetStdStringFromV8Object(args[0], name);
}


int Api::GetIntParam(const Arguments& args, const char* name){
	return GetIntFromV8Object(args[0], name);
}


Handle<Array> Api::GetV8ArrayParam(const Arguments& args, const char* name){
	 return GetV8ArrayFromV8Object(args[0], name);
}


Handle<Array> Api::DevicesToV8Array(std::vector<Device*> *devices, int deviceDataType){
	Handle<Array> array = Array::New((int) devices->size());

	for (unsigned int i = 0; i != devices->size(); ++i)
		array->Set(i, devices->at(i)->ToV8Object(deviceDataType, true));

	return array;
}


Handle<Object> Api::DevicesToV8Object(std::vector<Device*> *devices, int deviceDataType){
	Handle<Object> result = Object::New();

	for (unsigned int i = 0; i != devices->size(); ++i)
		AddPairToV8Object(result, devices->at(i)->GetStrId()->c_str(), devices->at(i)->ToV8Object(deviceDataType, false));

	return result;
}

