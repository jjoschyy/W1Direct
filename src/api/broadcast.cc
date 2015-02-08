#include "broadcast.h"

using namespace v8;

#define CP_COMMAND  "command"
#define CV_COMMAND  "convertTemperature"
#define CMD_TCONV	0x44


Handle<Value> Broadcast::BusCommand(const Arguments& args) {
	HandleScope scope;

	bool validArgs =
		AssertParamsFormat(args) 				     &&
		AssertDefaultParam(args, DP_MASTER_NAME)     &&
		AssertDefaultParam(args, DP_BUS_NUMBER)      &&
		AssertParam(args,   CP_COMMAND, DT_STRING)   &&
		AssertParamIn(args, CP_COMMAND, CV_COMMAND)  &&
		AssertMaster(args)						     &&
		AssertBus(args);


	if (validArgs){
		GetBus(args)->SetOverdriveSpeed(false);
		GetBus(args)->BroadcastCommand(CMD_TCONV);
	}


	return scope.Close(Undefined());
}


