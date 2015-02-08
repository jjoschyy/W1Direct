#include "read.h"
#include "../shared/util.h"
#include <vector>
#include <algorithm>

using namespace v8;

#define CP_FIELDS "fields"
#define CV_FIELDS "values|properties|connection"


Handle<Value> Read::DevicesById(const Arguments& args) {
	HandleScope scope;

	bool validArgs =
	  AssertParamsFormat(args) 				  	 	  &&
  	  AssertDefaultParam(args, DP_DEVICE_IDS) 	 	  &&
  	  AssertParam(args, CP_FIELDS, DT_ARRAY)   	 	  &&
  	  AssertArrayParamIn(args, CP_FIELDS, CV_FIELDS)  &&
	  AssertDevices(args);


	if (validArgs){
	   std::vector<Device*> devices = GetDevices(args);
	   Handle<Object> result = DevicesToV8Object(&devices, GetFieldBitMask(args));
	   return scope.Close(result);

	} else
	   return scope.Close(Undefined());

}


//private

int Read::GetFieldBitMask(const Arguments& args){

	int mask=0; std::string field;
	Handle<Array> fields = GetV8ArrayParam(args, CP_FIELDS);

	for (unsigned int i=0; i<fields->Length(); ++i){
		field = V8ValueToStdString(fields->Get(i));

		Util::AddToBitMaskIf(&mask, &field, "values", 	  DDT_VALUES);
		Util::AddToBitMaskIf(&mask, &field, "properties", DDT_PROPERTIES);
		Util::AddToBitMaskIf(&mask, &field, "connection", DDT_CONNECTION);
	}

	return mask;
}


