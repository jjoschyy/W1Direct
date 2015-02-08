#include "ds18s20.h"
#include "lib/temp.h"
#include "../master/bus/bus.h"
#include "../shared/v8_helper.h"
#include <stdint.h>
#include <string>

//COMMANDS
#define CMD_SCRATCHPAD_READ    0xBE
#define CMD_POWER_SUPPLY_READ  0xB4

//DATA-BYTES
#define DIX_TEMP_LSB		   0
#define DIX_TEMP_MSB		   1
#define DIX_COUNT_REMAIN	   6
#define DIX_COUNT_PER_C	 	   7
#define DIX_CRC8			   8
#define DIX_POWER_SUPPLY	   9

//RESOLUTION
#define PPC_RESOLUTION   	   0
#define INIT_RESOLUTION  	   "12bit"

using namespace v8;


Ds18s20::Ds18s20(Bus* bus, uint64_t intDeviceId, std::string* strDeviceId) : Device(bus, intDeviceId, strDeviceId){
	REGISTER_UPDATER(Ds18s20::UpdateResolution, "resolution", "9bit|10bit|11bit|12bit");
}


bool Ds18s20::Initialize(void){
	return UpdateResolution(INIT_RESOLUTION);
}


void Ds18s20::ReadValueData(void){
    Command(CMD_SCRATCHPAD_READ);
    ReadBytes(DIX_CRC8+1);
}


void Ds18s20::ReadPropertyData(void){
	Command(CMD_POWER_SUPPLY_READ);
    ReadByte(DIX_POWER_SUPPLY);
}


bool Ds18s20::VerifyValueData(void){
	return Crc8DataValidate(8, DIX_CRC8);
}


void Ds18s20::BuildValueData(Handle<Object> target){
	BuildTCelsius(target, "tCelsius");
}


void Ds18s20::BuildPropertyData(Handle<Object> target){
	V8Helper::AddPairToV8Object(target, "resolution",  "%ubit", propCache[PPC_RESOLUTION]);
	V8Helper::AddPairToV8Object(target, "powerSupply", data[DIX_POWER_SUPPLY] ? true : false);
}


bool Ds18s20::UpdateResolution(const char* strResolution){
	propCache[PPC_RESOLUTION] = Temp::ResolutionFromString(strResolution);
	return true;
}


//private

void Ds18s20::BuildTCelsius(Handle<Object> target, const char* targetKey){

	uint8_t tempDig, tempDec, subzero;

	//concat both values
	uint16_t meas = Temp::ConcatMsbLsb(data[DIX_TEMP_MSB], data[DIX_TEMP_LSB]);

	//calculate 12 bit resolution
	meas &= (uint16_t) 0xfffe;  		   //Discard LSB, needed for later extended precicion calc
	meas <<= 3;                 		   //Convert to 12-bit, now degrees are in 1/16 degrees units
	meas += (16-data[DIX_COUNT_REMAIN])-4; //Add the compensation and remember to subtract 0.25 degree (4/16)

	//twos complement if subzero
	Temp::HandleSubzero(&meas, &subzero);

	//extract temp
	tempDig = (uint8_t)(meas >> 4);
	tempDec = (uint8_t)(meas & 0x000F);

	const char* decimals = Temp::DecimalsToString(propCache[PPC_RESOLUTION], tempDec);
	V8Helper::AddPairToV8Object(target, targetKey, "%s%u.%s", subzero ? "-" : "", tempDig, decimals);

}



