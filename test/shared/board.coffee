exports.MASTER_NAME = 'MASTER'

exports.DS18S20 = '104C3D7101080061'
exports.DS18B20 = '28E445AA040000FC'
exports.DS2413a = '3A988D0C000000BA'
exports.DS2413b = '3A8D8E0C00000044'
exports.DS2408a = '29AD5712000000CE'
exports.DS2408b = '29A74A1200000091'


exports.SYNCED_DEVICES = 
 [{ id: @DS18S20, state: 'ready',       master: @MASTER_NAME, bus: 0, crcError: false },
  { id: @DS18B20, state: 'ready',       master: @MASTER_NAME, bus: 0, crcError: false },
  { id: @DS2413a, state: 'unsupported', master: @MASTER_NAME, bus: 0, crcError: false },
  { id: @DS2413b, state: 'unsupported', master: @MASTER_NAME, bus: 0, crcError: false },
  { id: @DS2408a, state: 'ready',       master: @MASTER_NAME, bus: 0, crcError: false },
  { id: @DS2408b, state: 'ready',       master: @MASTER_NAME, bus: 0, crcError: false }]

  
exports.newDS2482Params = ->
   name    : @MASTER_NAME
   subType : '100'
   devFile : '/dev/i2c-1'
   address : 0x18

