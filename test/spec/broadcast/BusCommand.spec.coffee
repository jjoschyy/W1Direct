w1direct  = require('./../../../build/Release/w1direct')
board     = require('../../shared/board')
paramTest = require('../../shared/params.spec')
w1        = undefined


describe "Broadcast::BusCommand", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
  )


  paramTest.testFor('broadcastBusCommand',
    masterName : 'String'
    busNumber  : 'Number'
    command    : 'String'
  )


  it 'should raise error on invalid master name', ->
    expect(-> w1.syncBusDevices({masterName:'invalid', busNumber:0, command:'convertTemperature'})).
      toThrow "The master 'invalid' has not been registered."


  it 'should raise error on invalid bus number', ->
    expect(-> w1.syncBusDevices({masterName:board.MASTER_NAME, busNumber:8, command:'convertTemperature'})).
      toThrow "The bus '8' on master '#{board.MASTER_NAME}' does not exist."

  
  it 'should raise error on invalid command', ->
    expect(-> w1.broadcastBusCommand({masterName:board.MASTER_NAME, busNumber:0, command:"invalidCommand"})).
      toThrow "Value 'invalidCommand' invalid for param 'command'. Allowed values: convertTemperature"


  it 'should broadcast >>convertTemperature<<', ->
    expect(w1.broadcastBusCommand({masterName:board.MASTER_NAME, busNumber:0, command:"convertTemperature"})).
      toBeUndefined()
