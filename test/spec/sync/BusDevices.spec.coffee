w1direct  = require('./../../../build/Release/w1direct')
board   = require('../../shared/board')
paramTest = require('../../shared/params.spec')
w1        = undefined


describe "Sync::BusDevices", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
  )
  
  
  paramTest.testFor('syncBusDevices',
    masterName : 'String'
    busNumber  : 'Number'
  )


  it 'should raise error on invalid master name', ->
    expect(-> w1.syncBusDevices({masterName:'invalid', busNumber:0})).
      toThrow "The master 'invalid' has not been registered."


  it 'should raise error on invalid bus number', ->
    expect(-> w1.syncBusDevices({masterName:board.MASTER_NAME, busNumber:8})).
      toThrow "The bus '8' on master '#{board.MASTER_NAME}' does not exist."


  it 'should sync bus devices two times', ->
    expect(w1.syncBusDevices({masterName:board.MASTER_NAME, busNumber:0})).toEqual(
      added   : board.SYNCED_DEVICES
      updated : []
      removed : []
    )   

    expect(w1.syncBusDevices({masterName:board.MASTER_NAME, busNumber:0})).toEqual(
      added   : []
      updated : board.SYNCED_DEVICES
      removed : []
    )   

