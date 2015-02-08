#ifndef REGISTER_H
#define REGISTER_H

#include "api.h"
#include <node.h>

using namespace v8;


class Register : public Api   {

public:
  static Handle<Value> DS2482Master(const Arguments&);


private:
  static bool DS2482MasterAssertParams(const Arguments&);
  static void DS2482MasterSetup(const Arguments&);

};


#endif
