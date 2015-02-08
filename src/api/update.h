#ifndef UPDATE_H
#define UPDATE_H

#include "api.h"
#include <node.h>

using namespace v8;


class Update: public Api   {

public:
  static Handle<Value> DeviceById(const Arguments&);

private:
  static bool AssertUpdaterExists(const Arguments&);
  static bool AssertUpdaterValue(const Arguments&);

  static std::string GetUpdaterName(const Arguments&);
  static bool ExecuteUpdater(const Arguments&);

};


#endif
