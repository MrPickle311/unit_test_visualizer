.import "../common/globalFunctions.js" as Global
.import com.myProject 1.0 as Port

function createSettignsWindow()
{
    return Global.createWindow("qrc:/script/settings_window/SettingsWindow.qml")
}

const bauds = new Map([
                          ["1200", Port.SerialPort.Baud1200    ] ,
                          ["2400",  Port.SerialPort.Baud2400   ] ,
                          ["4800",  Port.SerialPort.Baud4800   ] ,
                          ["9600",  Port.SerialPort.Baud9600   ] ,
                          ["19200", Port.SerialPort.Baud19200  ] ,
                          ["38400", Port.SerialPort.Baud38400  ] ,
                          ["57600", Port.SerialPort.Baud57600  ] ,
                          ["115200",Port.SerialPort.Baud115200 ]
                ]);

const stopBits = new Map([
                             ["1", Port.SerialPort.OneStop   ] ,
                             ["1.5",  Port.SerialPort.OneAndHalfStop   ] ,
                             ["2",  Port.SerialPort.TwoStop   ]
                   ]);

const dataBits = new Map([
                             ["5", Port.SerialPort.Data5    ] ,
                             ["6",  Port.SerialPort.Data6   ] ,
                             ["7",  Port.SerialPort.Data7   ] ,
                             ["8",  Port.SerialPort.Data8   ]
                   ]);

const parity = new Map([
                             ["None", Port.SerialPort.NoParity    ] ,
                             ["Odd",  Port.SerialPort.OddParity   ] ,
                             ["Even",  Port.SerialPort.EvenParity   ] ,
                             ["Space",  Port.SerialPort.SpaceParity   ] ,
                             ["Mark",  Port.SerialPort.MarkParity   ]
                   ]);
