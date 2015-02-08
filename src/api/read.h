#ifndef READ_H
#define READ_H

#include "api.h"
#include <node.h>

using namespace v8;


class Read: public Api   {

public:
  static Handle<Value> DevicesById(const Arguments&);


private:
  static int GetFieldBitMask(const Arguments&);


};


#endif
