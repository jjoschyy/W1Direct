w1direct   = require('./../../build/Release/w1direct')
board      = require('./board')
apiScope   = undefined
w1Synced   = undefined


RESOLUTION_DECIMALS = {
  "9bit":  [0,                                                     0.5                                                    ]
  "10bit": [0,                        0.25,                        0.5,                        0.75                       ]
  "11bit": [0,         0.125,         0.25,         0.375,         0.5,         0.625,         0.75,         0.875        ]
  "12bit": [0, 0.0625, 0.125, 0.1875, 0.25, 0.3125, 0.375, 0.4375, 0.5, 0.5625, 0.625, 0.6875, 0.75, 0.8125, 0.875, 0.9375]
}



class TestApi
  
  constructor: (testScope, testDeviceId) ->
    apiScope = @
    
    @w1 = @syncedW1Instance()
    @testDeviceId = testDeviceId
    testScope.addMatchers(@matchers())
  
  

  expectUpdate: (set, value, asCallback) ->
    if asCallback then expect(-> apiScope.updateDevice(set, value)) else expect(apiScope.updateDevice(set, value)) 
  
  
  expectRead: (key) ->
    expect(apiScope.readDevice()[key])
  
  
  expectUpdateAndRead: (set, value, readKey=set) ->
    apiScope.updateDevice(set, value)
    apiScope.expectRead(readKey)
  

  convertTemperatures: (waitTime, callback) ->
    apiScope.broadcastBusCommand("convertTemperature")
    waits(waitTime)
    runs(-> callback())
  
  

  #private
  
  syncedW1Instance: ->
    w1Synced = w1Synced || @newSyncedW1Instance()
    w1Synced  
  
  
  newSyncedW1Instance: ->
    w1 = new w1direct.Manager()
    w1.registerDS2482Master(board.newDS2482Params())
    w1.syncAllDevices()
    w1
  
  
  matchers: ->    
    toBeCorrectTemperature: (bits) -> 
      RESOLUTION_DECIMALS[bits].indexOf(@actual % 1) >= 0 && @actual < 40 && @actual > 0

    toHaveValidCrc: (bits) -> 
      @actual["crcError"] == false


  readDevice: ->
    @w1.readDevicesById({deviceIds:[@testDeviceId], fields:['values','properties']})[@testDeviceId]
  
  
  updateDevice: (set, value) ->
    @w1.updateDeviceById({deviceId:@testDeviceId, set:set, value:value})
  
  
  broadcastBusCommand: (command) ->
    @w1.broadcastBusCommand({masterName:board.MASTER_NAME, busNumber:0, command:command})
    
      
  


exports.TestApi = TestApi


