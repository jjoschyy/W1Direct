## Synopsis

NodeJS Addon for simple usage of 1wire over I2C and DS2482-100 and DS2482-800 master. Multiple masters and overdrive speed is supported. 

Target OS is LINUX with I2C module loaded. Currently only tested on Rasberry PI. The following devices are supported:

- DS18s20
- DS18b20
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
	name	  : 'i2c-1',        // Any name for later outputs
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
   [{ id: '104C3D7101080061',	//DS18s20
      state: 'ready',
      master: 'i2c-1',
      bus: 0,
      crcError: false },
    { id: '28E445AA040000FC',	//DS18b20
      state: 'ready',
      master: 'i2c-1',
      bus: 0,
      crcError: false },
    { id: '29AD5712000000CE',	//DS2408
      state: 'ready',
      master: 'i2c-1',
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
{ '104C3D7101080061':		//DS18s20
   { ioSpeed: 'standard',
     resolution: '12bit',
     powerSupply: true,
     tCelsius: '85.0',
     crcError: false },
  '28E445AA040000FC':		//DS18b20
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
In the result above, the temperature is "85.0". This is quite hot :-) To get the right temperature, each device has to calculate the right value, first.      



## License

MIT
