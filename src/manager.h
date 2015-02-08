#ifndef MANAGER_H
#define MANAGER_H

#include <node.h>
#include "controller/controller.h"


class Manager : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object>);
  Controller *controller;

 private:
  static v8::Handle<v8::Value> New(const v8::Arguments&);
  static void AddPrototype(v8::Handle<v8::FunctionTemplate>, const char*, v8::Handle<v8::Value>(*fn)(const v8::Arguments&));
};



#endif
