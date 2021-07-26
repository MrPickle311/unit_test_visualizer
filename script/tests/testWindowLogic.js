.import "../common/globalFunctions.js" as Global

function createTestsWindow()
{
    return Global.createWindow("qrc:/script/tests/TestWindow.qml")
}

function appendTestCase(test_case_name){
    model.append({"testCaseName" : test_case_name , "case_color" : "palegreen" , "collapsed"  : true ,"unitTests" : []  });
}

function setCurrentTestCaseFail(){
    model.get(model.count - 1).case_color = "tomato"
}

function appendUnitTest(test_case_nmbr , unit_test , is_range_test){
    model.get(test_case_nmbr).unitTests.append({"expressionName": unit_test.name ,
                                                "typeDescriptor": unit_test.typeDescriptor ,
                                                "currentValue"  : unit_test.currentValue ,

                                                "expectedValue" : is_range_test ?
                                                                      "A value between " + unit_test.lowerValue +
                                                                      " and " + unit_test.upperValue
                                                                    :
                                                                      unit_test.expectetedValue ,

                                                "testResult"    : unit_test.testResult} );
    if(unit_test.testResult === "Failed")
        setCurrentTestCaseFail()
}
