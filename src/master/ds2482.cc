#include "ds2482.h"
#include "../shared/util.h"
#include <string>
#include <stdio.h>

#ifdef _WIN32
	#include "lib/i2c-dev-win.h"
	#include "lib/io-win.h"
#else
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/ioctl.h>
	#include "lib/i2c-dev-linux.h"
#endif

#define CMD_RESET				0xF0
#define CMD_SET_READ_PTR		0xE1
#define CMD_CHANNEL_SELECT		0xC3
#define CMD_WRITE_CONFIG		0xD2

#define CMD_W1_RESET			0xB4
#define CMD_W1_SINGLE_BIT		0x87
#define CMD_W1_WRITE_BYTE		0xA5
#define CMD_W1_READ_BYTE		0x96
#define CMD_W1_TRIPLET			0x78

#define REG_STS_RST				0x10
#define REG_STS_LL				0x08
#define REG_STS_PPD				0x02
#define REG_STS_1WB				0x01
#define REG_CFG_1WS				0x08

#define PTR_CODE_STATUS			0xF0
#define PTR_CODE_DATA			0xE1
#define PTR_CODE_CHANNEL		0xD2
#define PTR_CODE_CONFIG         0xC3


//Bus-Channels for read/write
static const uint8_t channelsWr[8] = { 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87 };
static const uint8_t channelsRd[8] = { 0xB8, 0xB1, 0xAA, 0xA3, 0x9C, 0x95, 0x8E, 0x87 };



DS2482::DS2482(std::string* name, std::string* subType) : Master(name) {
	int busCount = subType->compare("100") == 0 ? 1 : 8;
	
	for(int i=0; i<busCount; i++)
		AddBus();
}


bool DS2482::Initialize(std::string* devFile, int address){

	masterFd = open(devFile->c_str(), O_RDWR);
	bool success = (masterFd != -1);
	Util::ThrowExceptionIf(!success, "Cannot open dev file '%s'", devFile->c_str());

	if (success){
		success = (ioctl(masterFd, I2C_SLAVE, address) != -1);
		Util::ThrowExceptionIf(!success, "Ioctl failed on '%s', 0x%x", devFile->c_str(), address);
	}

	if (success){
		success = SendCmd(CMD_RESET);
		Util::ThrowExceptionIf(!success, "Master not responding on '%s', 0x%x", devFile->c_str(), address);
	}

	return success;
}



void DS2482::SelectBus(Bus* bus){
	SendCmdWithData(CMD_CHANNEL_SELECT, channelsWr[bus->GetNumber()]);
}


void DS2482::SetOverdriveSpeed(bool enabled){
  ReadRegUntilW1Idle();
  uint8_t regValue = enabled ? REG_CFG_1WS : 0x00;
  SendCmdWithData(CMD_WRITE_CONFIG, CalculateConfig(regValue));
}


uint8_t DS2482::W1Reset(void){
	uint8_t reg;

	ReadRegUntilW1Idle();
	SendCmd(CMD_W1_RESET);

	reg = ReadRegUntilW1Idle();
	reg = !(reg & REG_STS_PPD);

	return reg;
}


void DS2482::W1WriteByte(uint8_t byte){
	ReadRegUntilW1Idle();
	SendCmdWithData(CMD_W1_WRITE_BYTE, byte);
}


uint8_t DS2482::W1ReadByte(void){

	ReadRegUntilW1Idle();
	SendCmd(CMD_W1_READ_BYTE);
	ReadRegUntilW1Idle();
	SelectRegister(PTR_CODE_DATA);

	return ReadByte();
}


uint8_t DS2482::W1Triplet(uint8_t dbit){

	ReadRegUntilW1Idle();
	SendCmdWithData(CMD_W1_TRIPLET, dbit ? 0xFF : 0);
	uint8_t status = ReadRegUntilW1Idle();

	return (status >> 5);
}


uint8_t DS2482::ReadRegUntilW1Idle(void){

	uint8_t reg;
	SelectRegister(PTR_CODE_STATUS);

	do {reg = ReadByte();}
	while ((reg & REG_STS_1WB));

	return reg;
}


uint8_t DS2482::ReadByte(void){
	return i2c_smbus_read_byte(masterFd);
}



uint8_t DS2482::CalculateConfig(uint8_t conf){
	return (conf | ((~conf & 0x0f) << 4));
}


bool DS2482::SelectRegister(uint8_t readPtr){
	return SendCmdWithData(CMD_SET_READ_PTR, readPtr);
}



bool DS2482::SendCmd(uint8_t cmd){
	return i2c_smbus_write_byte(masterFd, cmd) == 0;
}



bool DS2482::SendCmdWithData(uint8_t cmd, uint8_t data){
	return i2c_smbus_write_byte_data(masterFd, cmd, data) == 0;
}


