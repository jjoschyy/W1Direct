#ifndef SYNC_H
#define SYNC_H

#include "api.h"
#include "../controller/controller.h"
#include <node.h>

using namespace v8;


class Sync : public Api {

public:
  static Handle<Value> AllDevices(const Arguments&);
  static Handle<Value> MasterDevices(const Arguments&);
  static Handle<Value> BusDevices(const Arguments&);


private:
  static Handle<Object> ResponseSummary(DeviceStore* ds);
  static std::string MasterName(Handle<Value>);
  static int BusNumber(Handle<Value>);

};


#endif
