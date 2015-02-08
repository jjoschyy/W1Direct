## Synopsis

NodeJS Addon for simple usage of 1wire over I2C and DS2482-100 and DS2482-800 master. Multiple masters and overdrive speed is supported. 

Target OS is LINUX with I2C module loaded. Currently only tested on Rasberry PI. The following devices are supported:

- DS18b20
- DS18s20
- DS2408


## Code Example

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


## Motivation

A short description of the motivation behind the creation and maintenance of the project. This should explain **why** the project exists.

## Installation

Provide code examples and explanations of how to get the project.

## API Reference

Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. 

For medium size to larger projects it is important to at least provide a link to where the API reference docs live.

## Tests

Describe and show how to run the tests with code examples.

## Contributors

Let people know how they can dive into the project, include important links to things like issue trackers, irc, twitter accounts if applicable.

## License

A short snippet describing the license (MIT, Apache, etc.)

