#include "ds2408.h"
#include "../master/bus/bus.h"
#include "../shared/v8_helper.h"
#include "../shared/util.h"
#include "../shared/match.h"
#include "lib/usleep.h"
#include <stdint.h>
#include <string>
#include <bitset>
#include <cstring>
#include <cstdlib>


//COMMANDS
#define CMD_PIO_READ 				0xF0
#define CMD_CONDITIONAL_SREG_WRTIE  0xCC
#define CMD_CHANNEL_ACCESS_WRITE	0x5A
#define CMD_RESET_ACTIVITY_LATCHES	0xC3

//CACHE INDEXES
#define PPC_WRITE_MLTB_SUCCEED 	0

//RSTZ pin
#define RSTZ_KEY       		 "rstzPinMode"
#define RSTZ_VAL_STRB  		 "strobeOutput"
#define RSTZ_VAL_RESET 		 "resetInput"
#define RSTZ_DBIT	 		 2

//PIO
#define PIO_INPUT_KEY 		 "pioInput"
#define PIO_ACTIVITY_KEY	 "pioActivity"
#define PIO_OUTPUT_KEY		 "pioOutput"
#define PIO_OUTPUT_PORT_KEY	 "pioOutputPort"

//POWER SUPPLY
#define POWER_SUPPLY_KEY 	 "powerSupply"
#define POWER_SUPPLY_DBIT	 7

//DATA-BYTES
#define DIX_PIO_INPUT 		3
#define DIX_PIO_OUTPUT 		4
#define DIX_PIO_ACTIVITY	5
#define DIX_STATUS_REG    	8
#define DIX_CRC16_BYTE1   	11
#define DIX_CRC16_BYTE2   	12

using namespace v8;


Ds2408::Ds2408(Bus* bus, uint64_t intDeviceId, std::string* strDeviceId) : Device(bus, intDeviceId, strDeviceId){
	REGISTER_UPDATER(Ds2408::UpdateRstzPinMode,   RSTZ_KEY, RSTZ_VAL_STRB "|" RSTZ_VAL_RESET);
	REGISTER_UPDATER(Ds2408::UpdatePioOutput, 	  PIO_OUTPUT_KEY, MP_HEX_BYTE);
	REGISTER_UPDATER(Ds2408::UpdatePioOutputPort, PIO_OUTPUT_PORT_KEY, MP_PORT_VALUE);
	REGISTER_UPDATER(Ds2408::UpdatePioActivity,   PIO_ACTIVITY_KEY, "0x00");

	RegisterOverdriveUpdater();
}


void Ds2408::ReadAllData(void){

	Command(CMD_PIO_READ);

	//address (16bit)
	WriteByte(0x88);
	WriteByte(0x00);

	//needed for verify crc16
	data[0] = CMD_PIO_READ;
	data[1] = 0x88;
	data[2] = 0x00;

	//read 8 PIO bytes + 2 CRC bytes
	ReadBytes(3, 10);

	//crc bytes are inverted
	data[DIX_CRC16_BYTE1] = ~data[DIX_CRC16_BYTE1];
	data[DIX_CRC16_BYTE2] = ~data[DIX_CRC16_BYTE2];

}


bool Ds2408::VerifyAllData(void){
	return Crc16DataValidate(11, DIX_CRC16_BYTE1, DIX_CRC16_BYTE2);
}


void Ds2408::BuildValueData(Handle<Object> target){
	BuildValue(target, PIO_INPUT_KEY,    DIX_PIO_INPUT);
	BuildValue(target, PIO_OUTPUT_KEY,   DIX_PIO_OUTPUT);
	BuildValue(target, PIO_ACTIVITY_KEY, DIX_PIO_ACTIVITY);
}


void Ds2408::BuildPropertyData(Handle<Object> target){
	uint8_t srg = data[DIX_STATUS_REG];
	V8Helper::AddPairToV8Object(target, RSTZ_KEY, Util::BitIsSet(srg, RSTZ_DBIT) ? RSTZ_VAL_STRB : RSTZ_VAL_RESET);
	V8Helper::AddPairToV8Object(target, POWER_SUPPLY_KEY, Util::BitIsSet(srg, POWER_SUPPLY_DBIT));
}


bool Ds2408::UpdateRstzPinMode(const char* mode){

	uint8_t byte = strcmp(mode, RSTZ_VAL_STRB) == 0 ? 0x04 : 0x00;

	Command(CMD_CONDITIONAL_SREG_WRTIE);
	WriteByte(0x8D); //16 bit address part 1
	WriteByte(0x00); //16 bit address part 2
	WriteByte(byte);

	//Validate by read written
	Command(CMD_PIO_READ);
	WriteByte(0x8D);
	WriteByte(0x00);

	return ReadByte() == (byte + 0x80);

}


bool Ds2408::UpdatePioOutput(const char* hexStr){

	uint8_t byte = (uint8_t) strtol(hexStr+2, NULL, 16);
	return PiooWriteByte(byte);
}


bool Ds2408::UpdatePioOutputPort(const char* portStr){

	bool success  = false;
	uint8_t port  = portStr[1]-48;
	uint8_t value = portStr[3]-48;

	ReadAllData();

	if (VerifyAllData()){
		Util::SetBit(&data[DIX_PIO_OUTPUT], port, value == 1);
		success = PiooWriteByte(data[DIX_PIO_OUTPUT]);
	}

	return success;
}


bool Ds2408::UpdatePioActivity(const char* value){

	Command(CMD_RESET_ACTIVITY_LATCHES);
	return ReadByte() == 0xaa;
}




//private

bool Ds2408::PiooWriteByte(uint8_t byte){

	Command(CMD_CHANNEL_ACCESS_WRITE);
	WriteByte(byte);
	WriteByte(~byte);

	return ReadByte() == 0xaa;
}



void Ds2408::BuildValue(Handle<Object> target, const char* name, int dataIdx){
	Handle<Object> value = Object::New();
	V8Helper::AddPairToV8Object(value, "hex", "0x%02x", data[dataIdx]);
	V8Helper::AddPairToV8Object(value, "decimal", data[dataIdx]);
	V8Helper::AddPairToV8Object(value, "binary",  std::bitset<8>(data[dataIdx]).to_string().c_str());

	V8Helper::AddPairToV8Object(target, name, value);
}
