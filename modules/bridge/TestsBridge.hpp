#pragma once

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>
#include "../backend/PortSettings.hpp"
#include "../backend/ConvertedDataTypes.hpp"
#include "../backend/Interfaces.hpp"
#include "../backend/ParsedDataTypes.hpp"

namespace bridge
{

template<typename  T>
using Ptr = QSharedPointer<T>;

/// @brief This is base class  for a tests window direct binding
///
/// This abstract class provides also necessary signals to comunicate with UI
class TestsBody:
        public QObject
{
    Q_OBJECT
protected:
    QSharedPointer<QMetaObject::Connection> parser_connection_;//it exists to connect/disconnect signal
public:
    explicit TestsBody(QObject *parent = nullptr):
        QObject{parent},
        parser_connection_{new QMetaObject::Connection}
    {}
public slots:
    virtual void applySettings(QSerialPortInfo port , backend::PortFlowSettings settings) = 0;
    virtual void run()                                                                    = 0;
signals:
    /// This signal is used to sending test cases to QML
    /// @param test_case_name Name of test case to send
    void sendTestCase(QString test_case_name);
    /// This signal is used to sending unit test to QML
    /// @param test_case_nmbr This number indicates a test case number to which unit test belongs
    /// @param unit_test Unit test to send
    /// @param is_range_test Indicates that it is test which expected value is between upper and lower value
    void sendUnitTest(int test_case_nmbr ,  backend::UnitTest unit_test, bool is_range_test);
    /// This signal is emitted after an error has occurred
    /// @param error Error object to send
    void errorOccurred(const std::logic_error& error);
};

/// This value is sending to the device
/// so that repeat all process. If device
/// receive this value then resend all data
/// This value depends on in-device unit test
/// implementation
#define REPEAT_VALUE 51

/// @brief This class object is direct binding with terminal window in QML
///
/// It provides frontend <-> backend communication. Backend is placed by
/// composition
template<typename BufferType,
         typename PortOperatortType>
class Tests:
        public TestsBody
{
private:
    BufferType                       input_buffer_;
    BufferType                       output_buffer_;
    PortOperatortType                operator_;
    backend::TransactionDataPackage  data_result_;
    Ptr<interface::ParserComponent>  parser_;
    Ptr<interface::Converter>        converter_;
    backend::Transaction             result_;
private:
    /// After finished processing it sends all unit tests to QML list model
    void sendDataToFrontend()
    {
        bool is_range_test{false};
        for(int i{0} ; i < result_.cases_.size() ; ++i)
        {
            emit sendTestCase(result_.cases_[i].test_case_name_);

            for(auto&& unit_test : result_.cases_[i].tests_)
            {
                if(unit_test.expecteted_value_.isEmpty())
                    is_range_test = true;

                emit sendUnitTest(i,unit_test, is_range_test);
            }

            is_range_test = false;
        }
    }
    /// It only sets a buffer
    virtual void configureParser()
    {
        parser_->setBuffer(&input_buffer_);
    }
    /// This is engine which covers parsing , converting and sending data
    void process([[maybe_unused]] size_t count)
    {
        QObject::disconnect(*parser_connection_);///<- disconnecting prevents parser from
        try                                      /// unexpected auto-restarting
        {
            parser_->startProcessing();
            data_result_ = *parser_->getPackage();
            result_ = converter_->getConvertedTransaction();
            operator_.closePort();
            sendDataToFrontend();
        }
        catch (const std::logic_error& error)
        {
            clearAll();
            emit errorOccurred(error);
        }
    }
    /// Make and store connection to the buffer
    void makeConnections()
    {
        *parser_connection_ = QObject::connect(&input_buffer_ ,&interface::ByteBuffer::bytesArrived,
                                               this , &Tests::process  );
    }
    /// Clear all containters
    void clearAll()
    {
        data_result_.clear();
        result_.cases_.clear();
        converter_->reset();
    }
public:
    /// This method is invoked from UI by user when he confirm port settings.
    /// Tests module operates only on one port ,so ports can be changed flexible
    /// @param port Info about a port to set up
    /// @param settings New settings
    virtual void applySettings(QSerialPortInfo port, backend::PortFlowSettings settings) override
    {
        QString port_name{port.portName()};

        operator_.changeSettings(settings);
        operator_.changePort(port);
    }
    /// This method runs all parsing,converting algorithm.
    /// It communicates with the tested davice and pulls data from it.
    virtual void run() override
    {
        clearAll();

        try//two way of error reporting : by returing false or throwing an exception
        {
            if(operator_.openPort())
            {
                makeConnections();//single shot connection
                output_buffer_.appendByte(51);// i send 51 value
            }
            else throw std::logic_error{" Error occured at opening port : " +
                                        operator_.getError().toStdString() };
        }
        catch(const std::logic_error& error)
        {
            emit errorOccurred(error);
        }

    }
    /// This constructor applies pointer implementation dependencies which
    /// interfaces are compatible with interfaces in interface namespace
    /// @param converter Parsed data converter implementation
    /// @param parser Parser implementation
    /// @param parent QObjecy parent pointer
    Tests(QSharedPointer<interface::Converter> converter,
          QSharedPointer<interface::ParserComponent> parser,
          QObject* parent = nullptr):
        TestsBody{parent},
        converter_{converter},
        parser_{parser}
    {
        operator_.setInputByteBuffer(&input_buffer_);
        operator_.setOutputByteBuffer(&output_buffer_);
        configureParser();
        converter_->setPack(&data_result_);
    }
};

}
