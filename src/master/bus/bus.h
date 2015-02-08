#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <vector>

// "Bus" uses "Master" in header.
// Redefinition to prevent recursive include
class Master;

class Bus {

  public:
	Bus(Master*, int);

	Master* GetMaster();
	int		GetNumber();

	std::vector<uint64_t>  SearchDeviceIds(bool);
	void 	DeviceCommand(uint64_t, uint8_t);
	void 	BroadcastCommand(uint8_t);
	void	SetOverdriveSpeed(bool);

	void 	WriteByte(uint8_t);
	uint8_t ReadByte(void);
	uint8_t Triplet(uint8_t);
	uint8_t Reset();


  private:
	void 	Select(void);

	Master* master;
	int 	number;
	bool    overdriveSpeed;
};

#endif
