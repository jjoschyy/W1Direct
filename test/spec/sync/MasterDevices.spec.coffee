w1direct  = require('./../../../build/Release/w1direct')
board     = require('../../shared/board')
paramTest = require('../../shared/params.spec')
w1        = undefined


describe "Sync::MasterDevices", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
  )
  
  
  paramTest.testFor('syncMasterDevices',
    masterName : 'String'
  )


  it 'should raise error on invalid master name', ->
    expect(-> w1.syncMasterDevices({masterName:'invalid'})).toThrow "The master 'invalid' has not been registered."


  it 'should sync bus devices two times', ->
    expect(w1.syncMasterDevices({masterName:board.MASTER_NAME})).toEqual(
      added   : board.SYNCED_DEVICES
      updated : []
      removed : []
    )   

    expect(w1.syncMasterDevices({masterName:board.MASTER_NAME})).toEqual(
      added   : []
      updated : board.SYNCED_DEVICES
      removed : []
    )   
