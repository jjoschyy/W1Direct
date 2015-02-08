w1direct  = require('./../../../build/Release/w1direct')
board     = require('../../shared/board')
paramTest = require('../../shared/params.spec')
w1        = undefined


describe "Read::DevicesById", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
  )


  paramTest.testFor('readDevicesById',
    deviceIds : 'Array'
    fields    : 'Array'
  )

  
  it 'should raise error on not existing device', ->
    expect(-> w1.readDevicesById({fields:['values'], deviceIds:['invalid']})).
      toThrow "Device 'invalid' does not exist."


  it 'should raise error on not ready device', ->
    w1.syncAllDevices()
    expect(-> w1.readDevicesById({fields:['values'], deviceIds:[board.DS2413a]})).
      toThrow "Device '#{board.DS2413a}' is not in READY state."


  it 'should raise error on invalid field', ->
    expect(-> w1.readDevicesById({fields:['xx'], deviceIds:[board.DS18S20]})).
      toThrow "Value 'xx' invalid for array 'fields'. Allowed values: values|properties|connection"
