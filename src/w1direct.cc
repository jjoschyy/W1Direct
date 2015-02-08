#include <node.h>
#include "manager.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Manager::Init(exports);
}

NODE_MODULE(w1direct, InitAll)
