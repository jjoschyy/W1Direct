## Synopsis

NodeJS Addon for simple usage of 1wire over I2C and DS2482-100 and DS2482-800 master. Multiple masters and overdrive speed is supported. 

Target OS is LINUX with I2C module loaded. Currently only tested on Rasberry PI. The following devices are supported:

- DS18b20
- DS18s20
- DS2408


## Get started. Search your devices.

```js
var w1direct = require('w1direct');

w1 = new w1direct.Manager();

w1.registerDS2482Master({
	name	  : 'i2c-1',
	subType   : '100',
	devFile	  : '/dev/i2c-1',
	address	  : 0x18
});

console.log(w1.syncAllDevices());

{ added:
   [ { id: '104C3D7101080061',
       state: 'ready',
       master: 'i2c-1',
       bus: 0,
       crcError: false },
     { id: '28E445AA040000FC',
       state: 'ready',
       master: 'i2c-1',
       bus: 0,
       crcError: false }],
  
  updated: [],
  removed: []}


```


## API

Comming soon