#include "v8_helper.h"
#include <string>
#include <stdarg.h>

using namespace v8;
#define MAX_OBJECT_PAIR_STRING 32



std::string V8Helper::GetStdStringFromV8Object(Handle<Value> object, const char* key){
	return V8ValueToStdString(GetV8ValueFromV8Object(object, key));
}


int V8Helper::GetIntFromV8Object(Handle<Value> object, const char* key){
	return V8ValueToInt(GetV8ValueFromV8Object(object, key));
}


Handle<Value> V8Helper::GetV8ValueFromV8Object(Handle<Value> object, const char* key){
	return object->ToObject()->Get((Handle<String>) String::New(key));
}


Handle<Array> V8Helper::GetV8ArrayFromV8Object(Handle<Value> object, const char* key){
	return 	Handle<Array>::Cast(GetV8ValueFromV8Object(object, key));
}


std::string V8Helper::V8ValueToStdString(Handle<Value> value){
	String::Utf8Value str(value->ToString());
	return *str;
}


int V8Helper::V8ValueToInt(Handle<Value> value){
	Handle<Int32> intValue = value->ToInt32();
	return intValue->Value();
}


void V8Helper::AddPairToV8Object(Handle<Value> object, const char* key, std::string* value){
	object->ToObject()->Set(String::New(key), String::New(value->c_str()));
}


void V8Helper::AddPairToV8Object(Handle<Value> object, const char* key, const char* format, ...){

	char msg[MAX_OBJECT_PAIR_STRING+1];
	va_list args;
	va_start(args, format);
	vsnprintf(msg, MAX_OBJECT_PAIR_STRING, format, args);
	va_end(args);

	object->ToObject()->Set(String::New(key), String::New(msg));
}


void V8Helper::AddPairToV8Object(Handle<Value> object, const char* key, int value){
	object->ToObject()->Set(String::New(key), Number::New(value));
}


void V8Helper::AddPairToV8Object(Handle<Value> object, const char* key, bool value){
	object->ToObject()->Set(String::New(key), Boolean::New(value));
}


void V8Helper::AddPairToV8Object(Handle<Value> object, const char* key, Handle<Value> value){
	object->ToObject()->Set(String::New(key), value);
}


bool V8Helper::V8ObjectHasKey(Handle<Value> object, const char* key){
	return (GetStdStringFromV8Object(object, key).compare("undefined") != 0);
}


bool V8Helper::V8ValueIsFromDataType(Handle<Value> value, const char* key){

	return
	  (value->IsObject() && (key == (const char*) DT_OBJECT)) ||
	  (value->IsArray()  && (key == (const char*) DT_ARRAY))  ||
	  (value->IsString() && (key == (const char*) DT_STRING)) ||
	  (value->IsNumber() && (key == (const char*) DT_NUMBER));

}
