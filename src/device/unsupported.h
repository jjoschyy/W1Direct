#ifndef UNSUPPORTED_H
#define UNSUPPORTED_H

#include "device.h"
#include "stdint.h"
#include "../master/bus/bus.h"

using namespace v8;

class Unsupported : public Device {

  public:
	Unsupported(Bus*, uint64_t, std::string*);

};

#endif
