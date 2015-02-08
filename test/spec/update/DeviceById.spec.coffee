w1direct  = require('./../../../build/Release/w1direct')
board     = require('../../shared/board')
paramTest = require('../../shared/params.spec')
w1        = undefined


describe "Update::DeviceById", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
    w1.syncAllDevices()
  )


  paramTest.testFor('updateDeviceById',
    deviceId : 'String'
    set      : 'String'
    value    : 'String'
  )
  

  it 'should raise error on not existing device', ->
    expect(-> w1.updateDeviceById({deviceId:'invalid', set:'resolution', value:'9bit'})).
      toThrow "Device 'invalid' does not exist."


  it 'should raise error on invalid setter', ->
    expect(-> w1.updateDeviceById({deviceId:board.DS18B20, set:'invalidSetter', value:'9bit'})).
      toThrow "Update of 'invalidSetter' is not supported on device #{board.DS18B20}"


  it 'should raise error on invalid setter-value', ->
    expect(-> w1.updateDeviceById({deviceId:board.DS18B20, set:'resolution', value:'100bit'})).
      toThrow "Value '100bit' invalid for param 'value'. Allowed values: 9bit|10bit|11bit|12bit"

