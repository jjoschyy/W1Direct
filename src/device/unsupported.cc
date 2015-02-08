#include "unsupported.h"
#include "../master/bus/bus.h"
#include <stdint.h>
#include <string>

using namespace v8;


Unsupported::Unsupported(Bus* bus, uint64_t intDeviceId, std::string* strDeviceId) :
	Device(bus, intDeviceId, strDeviceId) {
	SetUnsupported();
}
