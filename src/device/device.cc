#include "device.h"
#include "../master/master.h"
#include "../shared/v8_helper.h"
#include "../shared/util.h"
#include "lib/crc.h"
#include <node.h>
#include <functional>
#include <string>

using namespace v8;

#define IO_SPEED_KEY   	  "ioSpeed"
#define IO_SPEED_VAL_STD  "standard"
#define IO_SPEED_VAL_OVD  "overdrive"


Device::Device(Bus* bus, uint64_t intDeviceId, std::string* strDeviceId)
    : bus(bus), intId(intDeviceId), strId(*strDeviceId), supported(true), overdriveSpeed(false)
{}


void Device::AfterNewSearched(uint64_t currentSyncId){
	syncId = currentSyncId;
	state = supported ? Initialize() ? STATE_READY : STATE_INITIALIZE_FAILED : STATE_UNSUPPORTED;
}


void Device::AfterAgainSearched(uint64_t currentSyncId){
	syncId = currentSyncId;
}


bool Device::IsReady(void){
	return (state == (char*) STATE_READY);
}


Bus* Device::GetBus(void){
	return bus;
}


uint64_t Device::GetIntId(void){
	return intId;
}


std::string* Device::GetStrId(void){
	return &strId;
}


uint64_t Device::GetSyncId(void){
	return syncId;
}


void Device::Command(uint8_t command){
	GetBus()->DeviceCommand(GetIntId(), command);
}


uint8_t Device::ReadByte(void){
	return GetBus()->ReadByte();
}


void Device::ReadByte(uint8_t dataIdx){
	data[dataIdx] = GetBus()->ReadByte();
}


void Device::ReadBytes(uint8_t byteCount){
	ReadBytes(0, byteCount);
}


void Device::ReadBytes(uint8_t dataStartIdx, uint8_t byteCount){
	for (int i=0; i<byteCount; i++)
		ReadByte(i + dataStartIdx);
}


void Device::WriteByte(uint8_t byte){
	GetBus()->WriteByte(byte);
}


bool Device::Crc8DataValidate(uint8_t buildByteCount, uint8_t idxCrc8Expected){
	return Crc::Validate8Bit(data, buildByteCount, data[idxCrc8Expected]);
}


bool Device::Crc16DataValidate(uint8_t buildByteCount, uint8_t idxByte1Crc16Expected, uint8_t idxByte2Crc16Expected){
	return Crc::Validate16Bit(data, buildByteCount, data[idxByte1Crc16Expected], data[idxByte2Crc16Expected]);
}


bool Device::SupportsUpdater(std::string* name){
	return (updaters.count(*name) == 1);
}


const char* Device::GetUpdaterValidator(std::string* name){
	return updaters[*name].validator;
}


bool Device::ExecuteUpdater(std::string* name, std::string* value){

	//enable overdrive support on updates
	GetBus()->SetOverdriveSpeed(overdriveSpeed);

	return updaters[*name].callback(value->c_str());
}


bool Device::UpdateOverdriveSpeed(const char* speed){
	overdriveSpeed = (speed[0] == 'o');
	return true;
}


//Required to sort devices for efficient read
bool Device::operator < (Device &device){
	return overdriveSpeed;
}


Handle<Object> Device::ToV8Object(int types, bool addDeviceId){

	Handle<Object> result = Object::New();

	if (addDeviceId)
		V8Helper::AddPairToV8Object(result, "id", GetStrId());

	//1. read
	ToV8ObjectReadData(types);

	//2. verify
	bool verified = ToV8ObjectVerifyData(types);

	//3. build
	if (verified)
		ToV8ObjectBuildData(types, result);

	//4. add error info
	V8Helper::AddPairToV8Object(result, "crcError", !verified);

	return result;
}



//protected

void Device::SetUnsupported(void){
	supported = false;
}


void Device::RegisterUpdater(std::function<bool(const char*)> callback, const char* name, const char* validation){
	Updater cb = {validation, callback};
	updaters[std::string(name)] = cb;
}


void Device::RegisterOverdriveUpdater(void){
	REGISTER_UPDATER(Device::UpdateOverdriveSpeed, IO_SPEED_KEY, IO_SPEED_VAL_STD "|" IO_SPEED_VAL_OVD);
}


//private

void Device::ToV8ObjectReadData(int types){

	//enable overdrive support on read
	GetBus()->SetOverdriveSpeed(overdriveSpeed);

	if (Util::BitIsMasked(types, DDT_PROPERTIES) || Util::BitIsMasked(types, DDT_VALUES))
		ReadAllData();

	if (Util::BitIsMasked(types, DDT_PROPERTIES))
		ReadPropertyData();

	if (Util::BitIsMasked(types, DDT_VALUES))
		ReadValueData();

}


bool Device::ToV8ObjectVerifyData(int types){

	bool verified = true;

	if (Util::BitIsMasked(types, DDT_PROPERTIES) || Util::BitIsMasked(types, DDT_VALUES))
		verified = VerifyAllData();

	if (Util::BitIsMasked(types, DDT_PROPERTIES))
		verified = verified && VerifyPropertyData();

	if (Util::BitIsMasked(types, DDT_VALUES))
		verified = verified && VerifyValueData();


	return verified;
}


void Device::ToV8ObjectBuildData(int types, Handle<Object> result){

	if (Util::BitIsMasked(types, DDT_CONNECTION))
		BuildConnectionData(result);

	if (Util::BitIsMasked(types, DDT_PROPERTIES)){
		V8Helper::AddPairToV8Object(result, IO_SPEED_KEY, overdriveSpeed ? IO_SPEED_VAL_OVD : IO_SPEED_VAL_STD);
		BuildPropertyData(result);
	}

	if (Util::BitIsMasked(types, DDT_VALUES))
		BuildValueData(result);

}


void Device::BuildConnectionData(Handle<Object> target){
	V8Helper::AddPairToV8Object(target, "state",  state);
	V8Helper::AddPairToV8Object(target, "master", GetBus()->GetMaster()->GetName());
	V8Helper::AddPairToV8Object(target, "bus",    GetBus()->GetNumber());
}
