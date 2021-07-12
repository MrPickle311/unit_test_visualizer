#pragma once

#include <QByteArray>
#include <QStateMachine>
#include <QState>
#include <QSignalTransition>

using Code = char;

enum  GlobalCommand{ START					 = 0 ,
                    SENDING_TEST_CASE        = 1 ,
                    SENDING_UNIT_TEST_RESULT = 2 ,
                    END_SENDING_TEST_CASE    = 3 ,
                    END_ENTIRE_TRANSACTION   = 4 };
/*
enum TestCaseCommand { SENDING_TEST_CASE        = 1 ,
}

*/
//it should be UnitTestCommand
enum  TestCaseCommand{ SENDING_NAME					= 0 ,
                      SENDING_TYPE_DESCRIPTOR		= 1 ,
                      SENDING_CURRENT_VALUE			= 2 ,
                      SENDING_EXPECTED_VALUE		= 3 ,
                      SENDING_TEST_RESULT			= 4 ,
                      SENDING_LINE_NMBR				= 5 ,
                      SENDING_LOWER_VALUE           = 6 ,
                      SENDING_UPPER_VALUE           = 7 ,
                      END_SENDING_UNIT_TEST_RESULT	= 8 };

enum  TypeDescriptor{ UINT8_T  = 0  ,
                     UINT16_T = 1  ,
                     UINT32_T = 2  ,
                     UINT64_T = 3  ,

                     INT8_T   = 4  ,
                     INT16_T  = 5  ,
                     INT32_T  = 6  ,
                     INT64_T  = 7  ,

                     BOOL     = 8  ,
                     CHAR     = 9  ,
                     PTR	  = 10 ,
                     BIT	  = 11 ,
                     TYPES_COUNT};


struct Command
{
    Code command_code_;
};

struct DataPackage
{
    QByteArrayList parsed_data_;
};

struct UnitTestPackage:
        public DataPackage
{
    QByteArray unit_test_name_;
    Code       type_descriptor_;
};

class LocalParser
{

};

class Parser
{

};


