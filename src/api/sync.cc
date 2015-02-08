#include "sync.h"
#include "../controller/controller.h"
#include <node.h>

using namespace v8;


Handle<Value> Sync::AllDevices(const Arguments& args) {
  HandleScope scope;

  Controller* ctl = GetController(args);
  DeviceStore* ds = ctl->GetDeviceStore();
  ds->ResetAllChanges(true);
  ctl->SyncAllDevices();

  return scope.Close(ResponseSummary(ds));
}



Handle<Value> Sync::MasterDevices(const Arguments& args) {
  HandleScope scope;

  Controller* ctl = GetController(args);
  DeviceStore* ds = ctl->GetDeviceStore();

  bool validArgs =
	AssertParamsFormat(args) &&
	AssertDefaultParam(args, DP_MASTER_NAME) &&
	AssertMaster(args);

  if (validArgs){
	  ds->ResetAllChanges(true);
 	  ctl->SyncMasterDevices(GetMaster(args));
 	  return scope.Close(ResponseSummary(ds));
  }

  return scope.Close(Undefined());
}



Handle<Value> Sync::BusDevices(const Arguments& args) {
  HandleScope scope;

  Controller* ctl = GetController(args);
  DeviceStore* ds = ctl->GetDeviceStore();

  bool validArgs =
	AssertParamsFormat(args) &&
    AssertDefaultParam(args, DP_MASTER_NAME) &&
    AssertDefaultParam(args, DP_BUS_NUMBER)  &&
	AssertMaster(args) &&
	AssertBus(args);

  if (validArgs){
	  ds->ResetAllChanges(true);
	  ctl->SyncBusDevices(GetBus(args));
	  return scope.Close(ResponseSummary(ds));
  }

  return scope.Close(Undefined());
}



//private

Handle<Object> Sync::ResponseSummary(DeviceStore* ds){

	Handle<Object> result = Object::New();
	result->Set(String::New("added"), 	DevicesToV8Array(ds->GetChanges(CHG_ADDED),   DDT_CONNECTION));
	result->Set(String::New("updated"), DevicesToV8Array(ds->GetChanges(CHG_UPDATED), DDT_CONNECTION));
	result->Set(String::New("removed"), DevicesToV8Array(ds->GetChanges(CHG_REMOVED), DDT_CONNECTION));

	return result;
}



