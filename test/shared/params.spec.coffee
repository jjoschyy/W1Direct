w1direct = require('./../../build/Release/w1direct')


exports.testFor = (fnName, paramDef) ->
  
  describe "(shared)", ->
    testParams = {}
    w1 = undefined

    beforeEach(-> 
      w1 = new w1direct.Manager()
    )
    
    
    resetTestParams = ->
      for param, dataType of paramDef
        testParams[param] = eval(dataType)()  


    it 'should validate missing param object', ->
      expect(-> w1[fnName]()).toThrow "First argument must be from data type 'object'"


    it 'should validate missing params', ->
      for param, dataType of paramDef
        resetTestParams()
        delete testParams[param]
        expect(-> w1[fnName](testParams)).toThrow "Param missing: #{param}"


    it 'should validate invalid data type', ->
      for param, dataType of paramDef
        resetTestParams()
        testParams[param] = {}
        expect(-> w1[fnName](testParams)).toThrow "Data type for param '#{param}' must be '#{dataType}'"
