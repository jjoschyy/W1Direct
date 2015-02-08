#include "ds18b20.h"
#include "lib/temp.h"
#include "../master/bus/bus.h"
#include "../shared/v8_helper.h"
#include <stdint.h>
#include <string>

//COMMANDS
#define CMD_SCRATCHPAD_READ    0xBE
#define CMD_SCRATCHPAD_WRITE   0x4E
#define CMD_POWER_SUPPLY_READ  0xB4

//DATA-BYTES
#define DIX_TEMP_LSB		   0
#define DIX_TEMP_MSB		   1
#define DIX_CONFIG_REGISTER    4
#define DIX_CRC8			   8
#define DIX_POWER_SUPPLY	   9

//RESOLUTION
#define PPC_RESOLUTION   	   0
#define INIT_RESOLUTION  	   "12bit"

using namespace v8;


Ds18b20::Ds18b20(Bus* bus, uint64_t intDeviceId, std::string* strDeviceId) : Device(bus, intDeviceId, strDeviceId){
	REGISTER_UPDATER(Ds18b20::UpdateResolution, "resolution", "9bit|10bit|11bit|12bit");
}


bool Ds18b20::Initialize(void){
	return UpdateResolution(INIT_RESOLUTION);
}


void Ds18b20::ReadValueData(void){
    Command(CMD_SCRATCHPAD_READ);
    ReadBytes(DIX_CRC8+1);
}


void Ds18b20::ReadPropertyData(void){
	Command(CMD_POWER_SUPPLY_READ);
    ReadByte(DIX_POWER_SUPPLY);
}


bool Ds18b20::VerifyValueData(void){
	return Crc8DataValidate(8, DIX_CRC8);
}


void Ds18b20::BuildValueData(Handle<Object> target){
	BuildTCelsius(target, "tCelsius");
}


void Ds18b20::BuildPropertyData(Handle<Object> target){
	V8Helper::AddPairToV8Object(target, "resolution",  "%ubit", propCache[PPC_RESOLUTION]);
	V8Helper::AddPairToV8Object(target, "powerSupply", data[DIX_POWER_SUPPLY] ? true : false);
}


bool Ds18b20::UpdateResolution(const char* strResolution){

	uint8_t resolution = Temp::ResolutionFromString(strResolution);

	//Set BIT 6,7 (00=9, 01=10, 10=11, 11=12)
	uint8_t cfgRegValue = ((resolution-9)*32)+31;
	WriteConfigRegister(cfgRegValue);

	if (VerifyConfigRegister(cfgRegValue)){
		propCache[PPC_RESOLUTION] = resolution;
		return true;
	}

	return false;
}


//private

void Ds18b20::BuildTCelsius(Handle<Object> target, const char* targetKey){

	uint8_t tempDig, tempDec, subzero;

	//concat both values
	uint16_t meas = Temp::ConcatMsbLsb(data[DIX_TEMP_MSB], data[DIX_TEMP_LSB]);

	//twos complement, if subzero
	Temp::HandleSubzero(&meas, &subzero);

	//extract temps
	tempDig = (uint8_t) (meas >> 4);
	tempDec = (uint8_t) (meas & 0x000F);

	//force zero on not relevant bits
	switch(propCache[PPC_RESOLUTION]) {
	  case 9:  tempDec &= 8;  break;  //9BIT  => 1000
	  case 10: tempDec &= 12; break;  //10BIT => 1100
	  case 11: tempDec &= 14; break;  //11BIT => 1110
	}

	const char* decimals = Temp::DecimalsToString(propCache[PPC_RESOLUTION], tempDec);
	V8Helper::AddPairToV8Object(target, targetKey, "%s%u.%s", subzero ? "-" : "", tempDig, decimals);
}


void Ds18b20::WriteConfigRegister(uint8_t value){

	Command(CMD_SCRATCHPAD_WRITE);
	WriteByte(0xFF);  //Th alarm
	WriteByte(0xFF);  //Tl alarm
	WriteByte(value); //CONFIG register
}


bool Ds18b20::VerifyConfigRegister(uint8_t shouldBe){

	ReadValueData();
	return VerifyValueData() && data[DIX_CONFIG_REGISTER] == shouldBe;
}


