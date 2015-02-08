#ifndef DEVICE_H
#define DEVICE_H

#include "../master/bus/bus.h"
#include <node.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <functional>
#include <map>

#define STATE_READY		 	  	 "ready"
#define STATE_INITIALIZE_FAILED  "initialize failed"
#define STATE_UNSUPPORTED 	  	 "unsupported"

#define DDT_CONNECTION 	1
#define DDT_PROPERTIES	2
#define DDT_VALUES		4

//Shortcut for RegisterUpdater function
#define REGISTER_UPDATER(fn, name, vld) RegisterUpdater(std::bind(&fn, this, std::placeholders::_1), name, vld)



typedef struct {
  const char* validator;
  std::function<bool(const char*)> callback;
} Updater;

using namespace v8;


class Device {

  public:
	Device(Bus*, uint64_t, std::string*);
	virtual ~Device(void){};

	//Initialize, for overwrite
	virtual bool Initialize(void){return true;}

	//Read functions, for overwrite
	virtual void ReadPropertyData(void){}
	virtual void ReadValueData(void){}
	virtual void ReadAllData(void){}

	//Verify functions, for overwrite
	virtual bool VerifyPropertyData(void) {return true;}
	virtual bool VerifyValueData(void)	  {return true;}
	virtual bool VerifyAllData(void)	  {return true;}

	//Build functions, for overwrite
	virtual void BuildPropertyData(Handle<Object>){}
	virtual void BuildValueData(Handle<Object>){}
	virtual void BuildConnectionData(Handle<Object>);

	void AfterNewSearched(uint64_t);
	void AfterAgainSearched(uint64_t);
	bool IsReady(void);

	Bus* GetBus(void);
	std::string* GetStrId(void);
	uint64_t GetIntId(void);
	uint64_t GetSyncId(void);

	void Command(uint8_t);
	uint8_t ReadByte(void);
	void ReadByte(uint8_t);
	void ReadBytes(uint8_t);
	void ReadBytes(uint8_t, uint8_t);
	void WriteByte(uint8_t);

	bool Crc8DataValidate(uint8_t, uint8_t);
	bool Crc16DataValidate(uint8_t, uint8_t, uint8_t);

	bool SupportsUpdater(std::string*);
	bool ExecuteUpdater(std::string*, std::string*);
	const char* GetUpdaterValidator(std::string*);

	bool UpdateOverdriveSpeed(const char*);
	bool operator < (Device&);
	Handle<Object> ToV8Object(int, bool);


  protected:
	void SetUnsupported(void);
	void RegisterUpdater(std::function<bool(const char*)>, const char*, const char*);
	void RegisterOverdriveUpdater(void);

	uint8_t data[32];
	uint8_t propCache[4];


  private:
	void ToV8ObjectReadData(int);
	bool ToV8ObjectVerifyData(int);
	void ToV8ObjectBuildData(int, Handle<Object>);

	Bus* bus;
	uint64_t intId;
	std::string strId;

	uint64_t syncId;
	const char* state;

	bool supported;
	bool overdriveSpeed;

	std::map<std::string, Updater> updaters;

};

#endif
