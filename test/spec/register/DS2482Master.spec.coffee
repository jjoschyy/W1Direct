w1direct  = require('./../../../build/Release/w1direct')
paramTest = require('../../shared/params.spec')
board     = require('../../shared/board')
w1        = undefined
params    = undefined


describe "Register::DS2482Master", ->

  beforeEach(-> 
    w1 = new w1direct.Manager()
    params = board.newDS2482Params()
  )
  

  paramTest.testFor('registerDS2482Master',
    name    : 'String'
    subType : 'String'
    devFile : 'String'
    address : 'Number'
  )


  it 'should register ds2482-100', ->
    w1.registerDS2482Master(params)

  
  it 'should register ds2482-800', ->
    params['subType'] = '800'
    w1.registerDS2482Master(params)

  
  it 'should raise error on invalid subType', ->
    params['subType'] = '900'
    expect(-> w1.registerDS2482Master(params)).toThrow "Value '900' invalid for param 'subType'. Allowed values: 100|800"


  it 'should raise error on invalid devFile', ->
    params['devFile'] = '/dev/not-there'
    expect(-> w1.registerDS2482Master(params)).toThrow "Cannot open dev file '/dev/not-there'"


  it 'should raise error on invalid address', ->
    params['address'] = 0x99
    expect(-> w1.registerDS2482Master(params)).toThrow "Ioctl failed on '/dev/i2c-1', 0x99"