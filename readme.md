## Synopsis

NodeJS Addon for simple usage of 1wire over I2C and DS2482-100 and DS2482-800 master. Multiple masters and overdrive speed is supported. 

Target OS is LINUX with I2C module loaded. Currently only tested on Rasberry PI. The following devices are supported:

- DS18S20
- DS18B20
- DS2408


## Prerequisites
On Rasberry Pi, the I2C module has to be loaded. The module is called "i2c_bcm2708" and should be defined in "/etc/modules". The communication speed is 100khz by default. Because the DS2482 supports 400khz, you can increase the speed by setting "i2c_bcm2708 baudrate=400000" in "/etc/modules". 

After rebooting your system, you should be able to see your DS2482 using  i2cdetect -y 1. 

## Install the package
Because the package is written in C++, it has to be compiled first. Node is doing this automatically using node-gyp. For installation, type:

```js
npm install w1direct
```


## Register your masters
You can register 1..n masters using the "registerDS2482Master" function:

```js
var w1direct = require('w1direct');

w1 = new w1direct.Manager();
w1.registerDS2482Master({
	name	  : 'MASTER1',      // Any name for later outputs
	subType   : '100',          // 100 or 800 for ds2482-100/800
	devFile	  : '/dev/i2c-1',   // The I2C device file
	address	  : 0x18            // The I2C master address (shown in i2cdetect)
});

```
## Search devices
Just execute:
```js
w1.syncAllDevices();
```
This returns the JSON below. If you do this action again, all devices will return inside the "updated" key.
```js
{ added:
   [{ id: '104C3D7101080061',	//DS18S20
      state: 'ready',
      master: 'MASTER1',
      bus: 0,
      crcError: false },
    { id: '28E445AA040000FC',	//DS18B20
      state: 'ready',
      master: 'MASTER1',
      bus: 0,
      crcError: false },
    { id: '29AD5712000000CE',	//DS2408
      state: 'ready',
      master: 'MASTER1',
      bus: 0,
      crcError: false }],
  
  updated: [],
  removed: []}
```

## Read devices
There are two possible types. The first is called "values", which holds values e.g. temperature. The second type is called "properties", which shows internal device properties. Reading both types needs more time. So normally you should only use the type you need.

Moreover, you can define multiple devices for read. Internally, this is performance optimized. So standard speed devices are read first. Afterwards the bus is switched to overdrive speed and all other devices are read.

```js
w1.readDevicesById({fields:['values', 'properties'], deviceIds:['104C3D7101080061', '28E445AA040000FC', '29AD5712000000CE']}
```
This returns:

```js
{ '104C3D7101080061':		//DS18S20
   { ioSpeed: 'standard',
     resolution: '12bit',
     powerSupply: true,
     tCelsius: '85.0',
     crcError: false },
  '28E445AA040000FC':		//DS18B20
   { ioSpeed: 'standard',
     resolution: '12bit',
     powerSupply: true,
     tCelsius: '85.0',
     crcError: false },
  '29AD5712000000CE':		//DS2408
   { ioSpeed: 'standard',
     rstzPinMode: 'resetInput',
     powerSupply: true,
     pioInput: { hex: '0x1f', decimal: 31, binary: '00011111' },
     pioOutput: { hex: '0xff', decimal: 255, binary: '11111111' },
     pioActivity: { hex: '0x00', decimal: 0, binary: '00000000' },
     crcError: false } }
```


## Broadcast devices
In the result above, the temperature is "85.0". This is quite hot :-) To read the right temperature, each device has to calculate the temperature first. To start this calculation, a "broadcast" command can be send:

```js
w1.broadcastBusCommand({masterName:'MASTER1', busNumber:0, command:"convertTemperature"})     
```

After the command is send, all devices on the specified Master/Bus start to build a memory entry with the current temperature. After a delay, the correct temperature can be read. The delays are different for each device and resolution. Details are shown below in the device section. 


## Update devices
For each device, there are some possible updates. Details are shown below in the device section.

```js
w1.updateDeviceById({deviceId:'DEVICE_ID', set:'KEY', value:'VALUE'})
```


## DS18S20 and DS18B20
###  Values and properties

```js 	
 { ioSpeed	   : 'standard', //property
   resolution  : '12bit',    //property
   powerSupply : true,       //property
   tCelsius	   : '85.0'  	 //value   
 }
```


### Updates

```js
w1.updateDeviceById({deviceId:'DEVICEID', set:'resolution', value:'9bit'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'resolution', value:'10bit'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'resolution', value:'11bit'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'resolution', value:'12bit'})
```

A higher resolution will give you more decimal values. For the DS18S20 the decimals are interpolated. The possible decimals are: 

```js
{
  "9bit":  [0,                                                     0.5                                                    ]
  "10bit": [0,                        0.25,                        0.5,                        0.75                       ]
  "11bit": [0,         0.125,         0.25,         0.375,         0.5,         0.625,         0.75,         0.875        ]
  "12bit": [0, 0.0625, 0.125, 0.1875, 0.25, 0.3125, 0.375, 0.4375, 0.5, 0.5625, 0.625, 0.6875, 0.75, 0.8125, 0.875, 0.9375]
}
```

The calculation delays are:

```js
{		  //DS18S20, DS18B20
  "9bit":  [750ms,   94ms],
  "10bit": [750ms,   188ms],
  "11bit": [750ms,   375ms],
  "12bit": [750ms,   750ms]
}
```


## DS2408
###  Values and properties

```js
{ 	ioSpeed     : 'standard',   //property
    rstzPinMode : 'resetInput', //property
    powerSupply : true,         //property  
    pioInput    : { hex: '0x1f', decimal: 31,  binary: '00011111' },  //value
    pioOutput   : { hex: '0xff', decimal: 255, binary: '11111111' },  //value
    pioActivity : { hex: '0x00', decimal: 0,   binary: '00000000' }   //value
}
```

###  Updates

```js
//Communication speed
w1.updateDeviceById({deviceId:'DEVICEID', set:'ioSpeed', value:'standard'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'ioSpeed', value:'overdrive'})

//The RSTZ pin can be RESET input or STROBE output
w1.updateDeviceById({deviceId:'DEVICEID', set:'rstzPinMode', value:'strobeOutput'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'rstzPinMode', value:'resetInput'})

//Sets the PIO output to 0xff and 0xbb
w1.updateDeviceById({deviceId:'DEVICEID', set:'pioOutput', value:'0xff'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'pioOutput', value:'0xbb'})

//Sets PIO Pin 0 to 1 and Pin 2 to 0
w1.updateDeviceById({deviceId:'DEVICEID', set:'pioOutputPort', value:'p0,1'})
w1.updateDeviceById({deviceId:'DEVICEID', set:'pioOutputPort', value:'p2,0'})

//Reset the activity latch
w1.updateDeviceById({deviceId:'DEVICEID', set:'pioActivity', value:'0x00'})

```


## License

MIT
