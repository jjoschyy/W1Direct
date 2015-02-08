w1direct = require('./../../../build/Release/w1direct')
board    = require('../../shared/board')
w1       = undefined


describe "Sync::AllDevices", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
  )

  it 'should sync all devices two times', ->
    expect(w1.syncAllDevices()).toEqual(
      added   : board.SYNCED_DEVICES
      updated : []
      removed : []
    )   

    expect(w1.syncAllDevices()).toEqual(
      added   : []
      updated : board.SYNCED_DEVICES
      removed : []
    )   
