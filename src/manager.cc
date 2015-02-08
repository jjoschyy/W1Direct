#include "manager.h"
#include "controller/controller.h"
#include "api/broadcast.h"
#include "api/read.h"
#include "api/register.h"
#include "api/sync.h"
#include "api/update.h"

#include <string>

using namespace v8;


void Manager::Init(Handle<Object> target) {

  // Prepare constructor template
  Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Manager"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Supported functions
  AddPrototype(tpl, "broadcastBusCommand", 	Broadcast::BusCommand);
  AddPrototype(tpl, "readDevicesById",	 	Read::DevicesById);
  AddPrototype(tpl, "registerDS2482Master", Register::DS2482Master);
  AddPrototype(tpl, "syncAllDevices", 		Sync::AllDevices);
  AddPrototype(tpl, "syncMasterDevices", 	Sync::MasterDevices);
  AddPrototype(tpl, "syncBusDevices",	 	Sync::BusDevices);
  AddPrototype(tpl, "updateDeviceById",	 	Update::DeviceById);

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("Manager"), constructor);
}



Handle<Value> Manager::New(const Arguments& args) {
  Manager* manager = new Manager();
  manager->controller = new Controller();
  manager->Wrap(args.This());

  return args.This();
}


void Manager::AddPrototype(Handle<FunctionTemplate> tpl, const char* fnName, Handle<Value> (*fn)(const Arguments&)){
	tpl->PrototypeTemplate()->Set(String::NewSymbol(fnName), FunctionTemplate::New(fn)->GetFunction());
}
