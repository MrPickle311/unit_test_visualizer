#pragma once

#include "PortSettings.hpp"
#include <QEventLoop>
#include "Interfaces.hpp"

namespace backend
{

/// @brief Objects of this class are buffers which can be shared and modified by several objects.
///
/// This buffer stores array of bytes which is dynamically resized. This buffer is often shared object,
/// so many objects may receive and append data from/to buffer. Objects of this class are threadsafe.
class ByteBuffer :
        public interface::ByteBuffer,
        public global::ProgramObject
{
    Q_OBJECT;
private:
    QByteArray received_bytes_;
    QMutex     data_mutex_;
    QEventLoop loop_;//synchronization
private:
    /// This function deteaches bytes before position argument
    /// @param position Indicates position to which bytes will be deteached
    /// @return Returns deteached array of bytes
    QByteArray splitByteArray(size_t position);
    /// This function wait for arriving bytes from RS232 port.
    /// It is useful with slower connections.
    void       waitForData();
public:
    /// Default constructor. Creates empty bytes array
    /// @param parent QObject parent
    ByteBuffer(QObject *parent = nullptr);
    virtual    ~ByteBuffer(){}
    /// This method appends bytes to the buffer
    /// @param array Bytes to be appended
    void       appendBytes(const QByteArray& array);
    /// This method appends a byte to the buffer
    /// @param byte Byte to be appended
    void       appendByte(char byte);
    /// This method flushes the buffer
    /// @return All bytes that were in the buffer
    QByteArray getAllBytes()                        noexcept;
    /// This method returns several bytes from buffer.
    /// @param count Count of requested bytes
    /// @return Array of bytes with count size
    /// @warning This method returns count bytes or throws
    QByteArray getBytes(size_t count);
    /// This method returns a byte from buffer.
    /// @return Returns a single byte
    /// @warning This method returns a byte or throws
    char       getByte();
    ///@return If buffer is empty returns true
    bool       isEmpty()                            const;
    ///@return Returns currently count of bytes in the buffer
    size_t     size()                               const;
};

/// @brief This base class provides basic RS232 port operators.
///
/// This class is a base for more advanced port operators. Port Operator
/// is a object which control and monitor a port life time
class PortStateOperator :
        public QObject,
        public global::ProgramObject
{
    Q_OBJECT;
protected:
    QSerialPort     current_port_;//physical port object
    QSerialPortInfo current_port_info_;//port information
private:
    /// This function makes basic Qt connections.
    void makeConnections();
public:
    /// Default constructor
    /// @param parent QObject parent
    PortStateOperator(QObject* parent = nullptr);
    /// Explicit specified constructor.
    /// @param settings Settings which port will apply
    /// @param port Information about port
    /// @param parent QObject parent
    PortStateOperator(PortFlowSettings settings,
                      QSerialPortInfo port ,
                      QObject* parent = nullptr);
    virtual ~PortStateOperator(){}
public slots:
    /// Changes/sets current port basing on information about port
    /// @param port Information about port
    void    changePort(QSerialPortInfo port);
    /// @param settings Settings to be applied
    void    changeSettings(PortFlowSettings settings);
    /// This method close a current port. If port is not opened
    /// nothing will happen.
    void    closePort();
    /// This method tries to open the current port.
    /// @return Returns true if this operation is successful
    /// @warning If port is already open , invocation this method
    /// will throw
    bool    openPort();
    /// @return Returns true if this port is open
    bool    isOpen()                                    const;
    /// @return Returns serialized current error description
    QString getError()                                  const;
protected slots:
    /// Accepts error enum value , converts it to string and
    /// re-emits signal to external  connected objects
    /// @param error Error enum to be converted to string
    void deviceErrorService(QSerialPort::SerialPortError error);
signals:
    ///  Emits signal with serialized error value
    /// @param what Serialized error value
    void deviceErrorOccurred(QString what);
};

/// @brief Extended PortStateOperator which allows to exchange data
///
/// This class extension has methods to get/send data from/to port directly.
class PortFlowOperator:
        public PortStateOperator
{
    Q_OBJECT;
private:
    /// This method makes connections between port and this class objects
    void makeConnections();
public:
    /// Default constructor
    /// @param parent QObject parent
    PortFlowOperator( QObject* parent = nullptr);
    /// Explicit specified constructor.
    /// @param settings Settings which port will apply
    /// @param port Information about port
    /// @param parent QObject parent
    PortFlowOperator( PortFlowSettings settings ,
                      QSerialPortInfo  port     ,
                      QObject* parent = nullptr);
    virtual ~PortFlowOperator(){}
public:
    /// This method allows to sending bytes to the port directly
    /// @param array Bytes to be send
    void       sendBytesToPort(const QByteArray& array);
    /// @return Returns all avalabile bytes from port directly.
    QByteArray getAllBytesFromPort();
signals:
    /// This signal is emmited when bytes arrive to the port
    void dataArrived();//external signal ,for user , data <- serial
    /// This signal is emmited when bytes are sent to the port
    void dataSent();//external signal ,for user data -> serial
};


/// @brief Extended PortFlowOperator which operates on the buffers
///
/// Objects of this class are connected with input and output buffers.
/// Input buffer aggregates data from port, user can get bytes from it.
/// Output buffer is used to sending data to port in convenient way
class BufferedPortFlowOperator:
        public PortFlowOperator
{
    Q_OBJECT;
protected:
    ByteBuffer* input_byte_buffer_;
    ByteBuffer* output_byte_buffer_;
private:
    void makeInputBufferConnections();
    void makeOutputBufferConnections();
    /// Check whether buffer is nullptr.
    /// If buffer is nullptr throws std::logic_error
    /// @param buffer Buffer to be test
    void checkBuffer(ByteBuffer* buffer) noexcept(false);
public:
    /// Default constructor
    /// @param parent QObject parent
    BufferedPortFlowOperator(QObject* parent = nullptr);
    /// Explicit specified constructor.
    /// @param settings Settings which port will apply
    /// @param port Information about port
    /// @param parent QObject parent
    BufferedPortFlowOperator( PortFlowSettings settings ,
                              QSerialPortInfo  port     ,
                              QObject* parent = nullptr);
    virtual ~BufferedPortFlowOperator(){}
protected slots:
    /// This method is used to automatically sending data from
    /// port to the input buffer
    void sendDataFromPortToBuffer();
    /// This method is used to automatically sending data from
    /// output buffer to the port
    void sendDataFromBufferToPort();
public:
    /// This method allows to set and connect input buffer
    /// @param byte_buffer Buffer to be set as input buffer
    void setInputByteBuffer(ByteBuffer* byte_buffer);
    /// This method allows to set and connect output buffer
    /// @param byte_buffer Buffer to be set as output buffer
    void setOutputByteBuffer(ByteBuffer* byte_buffer);
};

}
