.import "../common/globalFunctions.js" as Global

function createTestsWindow()
{
    return Global.createWindow("qrc:/script/tests/TestWindow.qml")
}

function TestsLogic(list_model){
    this.model = list_model;

    this.appendTestCase = function (test_case_name){
        console.log(this.model)
        this.model.append({"testCaseName" : test_case_name , "collapsed" : true ,"unitTests" : []  })
    }

    this.appendUnitTest = function (test_case_nmbr , unit_test , is_range_test){
        console.log(this.model)
        this.model.get(test_case_nmbr).unitTests.append({"expressionName": unit_test.name ,
                                                              "typeDescriptor": unit_test.typeDescriptor ,
                                                              "currentValue"  : unit_test.currentValue ,

                                                              "expectedValue" : is_range_test ?
                                                                                    "A value between " + unit_test.lowerValue +
                                                                                    " and " + unit_test.upperValue
                                                                                  :
                                                                                    unit_test.expectetedValue ,

                                                              "testResult"    : unit_test.testResult} );
    }
}

//class TestsLogic{
//    constructor (list_model){
//        console.log(list_model)
//        this.model = list_model
//        console.log(this.model)
//        this.model.append({"testCaseName" : "xd" , "collapsed" : true ,"unitTests" : []  })
//        this.nmbr = 5
//    }
//
//
//};
