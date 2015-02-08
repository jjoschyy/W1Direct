#ifndef BROADCAST_H
#define BROADCAST_H

#include "api.h"
#include <node.h>

using namespace v8;


class Broadcast: public Api   {

public:
  static Handle<Value> BusCommand(const Arguments&);


};


#endif
