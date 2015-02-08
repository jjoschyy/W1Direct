board      = require('../../shared/board')
TestApi    = require('../../shared/api').TestApi 
tapi       = undefined


describe('Test DS18S20', ->
  
  beforeEach(-> 
    tapi = new TestApi(@, board.DS18B20) 
  )


  testValues = (resolution, waitTime) ->
    tapi.expectUpdate("resolution", resolution).toHaveValidCrc()
    tapi.convertTemperatures(waitTime, -> tapi.expectRead("tCelsius").toBeCorrectTemperature(resolution))
    

  it 'read values in 9  bit', -> testValues('9bit',  750)
  it 'read values in 10 bit', -> testValues('10bit', 750)
  it 'read values in 11 bit', -> testValues('11bit', 750)
  it 'read values in 12 bit', -> testValues('12bit', 750)
  

  it 'read static properties', ->
    tapi.expectRead("powerSupply").toBe(true)
    tapi.expectRead("ioSpeed").toBe("standard") 
  

)


