board    = require('../../shared/board')
TestApi  = require('../../shared/api').TestApi 
tapi     = undefined


describe('Test DS2408', ->

  
  beforeEach(-> 
    tapi = new TestApi(@, board.DS2408a) 
  )

  
  it 'read pioInput', ->
    tapi.expectRead("pioInput").toEqual({ hex : '0x14', decimal : 20, binary : '00010100' })

  
  it 'update and read ioSpeed', ->
    tapi.expectUpdateAndRead("ioSpeed", "standard").toBe("standard")
    tapi.expectUpdateAndRead("ioSpeed", "overdrive").toBe("overdrive")
    tapi.expectUpdate("ioSpeed", "badValue", true).toThrow("Value 'badValue' invalid for param 'value'. Allowed values: standard|overdrive")

    
  it 'update and read rstzPinMode', ->
    tapi.expectUpdateAndRead("rstzPinMode", "strobeOutput").toBe("strobeOutput")
    tapi.expectUpdateAndRead("rstzPinMode", "resetInput").toBe("resetInput")
    tapi.expectUpdate("rstzPinMode", "badValue", true).toThrow("Value 'badValue' invalid for param 'value'. Allowed values: strobeOutput|resetInput")


  it 'update and read pioOutputs', ->
    tapi.expectUpdate("rstzPinMode", "strobeOutput").toHaveValidCrc()
    tapi.expectUpdateAndRead("pioOutput", "0xee").toEqual({hex:"0xee", decimal:238, binary:'11101110'})
    tapi.expectUpdate("pioOutput", "0xgg", true).toThrow("Value '0xgg' invalid for param 'value'. Allowed values: {0x??}")
    

  it 'update and read pioOutputPort', ->
    tapi.expectUpdate("rstzPinMode", "strobeOutput").toHaveValidCrc()
    tapi.expectUpdate("pioOutput", "0x00").toHaveValidCrc()
    tapi.expectUpdateAndRead("pioOutputPort", "p0,1", "pioOutput").toEqual({hex:"0x01", decimal:1, binary:'00000001'})
    tapi.expectUpdateAndRead("pioOutputPort", "p0,0", "pioOutput").toEqual({hex:"0x00", decimal:0, binary:'00000000'})
    tapi.expectUpdate("pioOutputPort", "p0,9", true).toThrow("Value 'p0,9' invalid for param 'value'. Allowed values: {p0-7,0|1}")


  it 'update and read pioActivity', ->
    tapi.expectUpdate("rstzPinMode", "strobeOutput").toHaveValidCrc()
    tapi.expectUpdate("pioOutput", "0xff").toHaveValidCrc()
    tapi.expectUpdateAndRead("pioActivity", "0x00").toEqual({hex:"0x00", decimal:0, binary:'00000000'})
    

  it 'read static properties', ->
    tapi.expectRead("powerSupply").toBe(true)
  

)
