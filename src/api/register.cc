#include "register.h"
#include "../manager.h"
#include "../master/ds2482.h"
#include "../controller/controller.h"
#include <string>
#include <stdio.h>

using namespace v8;

#define CP_NAME	    "name"
#define CP_DEV_FILE "devFile"
#define CP_ADDRESS  "address"
#define CP_SUBTYPE  "subType"
#define CV_SUBTYPE  "100|800"


Handle<Value> Register::DS2482Master(const Arguments& args) {
	HandleScope scope;

	if (DS2482MasterAssertParams(args))
	    DS2482MasterSetup(args);

	return scope.Close(Undefined());
}


//private

bool Register::DS2482MasterAssertParams(const Arguments& args){

  return
	AssertParamsFormat(args) 					&&
	AssertParam(args, CP_NAME, 	    DT_STRING)  &&
    AssertParam(args, CP_SUBTYPE,   DT_STRING)  &&
    AssertParam(args, CP_DEV_FILE,  DT_STRING)  &&
    AssertParam(args, CP_ADDRESS,   DT_NUMBER)  &&
    AssertParamIn(args, CP_SUBTYPE, CV_SUBTYPE);

}


void Register::DS2482MasterSetup(const Arguments& args){
	std::string masterName = GetStrParam(args, CP_NAME);
    std::string devFile    = GetStrParam(args, CP_DEV_FILE);
	std::string subType    = GetStrParam(args, CP_SUBTYPE);
    int devTargetAddress   = GetIntParam(args, CP_ADDRESS);
    DS2482 *ds2482Master   = new DS2482(&masterName, &devFile);


	if (ds2482Master->Initialize(&devFile, devTargetAddress)){
	    Manager* manager = node::ObjectWrap::Unwrap<Manager>(args.This());
		manager->controller->AddMaster(ds2482Master->GetName(), ds2482Master);
	}

}



