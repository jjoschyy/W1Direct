#ifndef V8_HELPER_H
#define V8_HELPER_H

#include <node.h>
#include <string>

#define DT_OBJECT "Object"
#define DT_ARRAY  "Array"
#define DT_STRING "String"
#define DT_NUMBER "Number"

using namespace v8;


class V8Helper {

  public:
	static std::string GetStdStringFromV8Object(Handle<Value>, const char*);
	static int GetIntFromV8Object(Handle<Value>, const char*);
	static Handle<Value> GetV8ValueFromV8Object(Handle<Value>, const char*);
	static Handle<Array> GetV8ArrayFromV8Object(Handle<Value>, const char*);


	static std::string V8ValueToStdString(Handle<Value>);
	static int V8ValueToInt(Handle<Value>);

	static void AddPairToV8Object(Handle<Value>, const char*, std::string*);
	static void AddPairToV8Object(Handle<Value>, const char*, const char*, ...);
	static void AddPairToV8Object(Handle<Value>, const char*, int);
	static void AddPairToV8Object(Handle<Value>, const char*, bool);

	static void AddPairToV8Object(Handle<Value>, const char*, Handle<Value>);

	static bool V8ObjectHasKey(Handle<Value>, const char*);
	static bool V8ValueIsFromDataType(Handle<Value>, const char*);

};

#endif
