#pragma once

#include "ParsedDataTypes.hpp"
#include <variant>
#include "../global/StaticGenerator.hpp"
#include "Interfaces.hpp"

namespace global
{

using backend::TypesSizes;
using backend::TypeDescriptor;

template<>
void TypesSizes::initValues();

}

namespace backend
{

//aliases

using interface::AcceptedTypes;

using UnitTestPackPtr    = QSharedPointer<UnitTestDataPackage>;
using TestCasePackPtr    = QSharedPointer<TestCaseDataPackage>;
using TransactionPackPtr = QSharedPointer<TransactionDataPackage>;


/// @brief This is the base class for all parsers in the parsing tree.
///
/// Parsing tree uses composite pattern ,so all incoming commands and data are
/// processing recursively in parseCommand() method.
/// Object of this type represents leaf of the parsing tree, so adding children
/// to it has no effect
class ParserComponent:
        public interface::ParserComponent
{
    friend class ComplexParser;
protected:
    /// Parent of the composite object.
    interface::ParserComponent* parent_;
    /// Buffer from which composite object takes bytes
    /// and parses it
    interface::ByteBuffer*      buffer_;
    /// A package which is appending to the parent package
    /// @see interface::AcceptedTypes
    AcceptedTypes               package_;
    /// This describes type of currently parsing unit test
    /// This is necessary to know the size(in bytes)
    static TypeDescriptor       current_type_;
public:
    /// @return Returns a pointer to the parent composite object
    virtual interface::ParserComponent* getParent()                                                              override;
    /// Sets a new aprent for this composite object
    /// @param newParent Parent to be set
    virtual void                        setParent(interface::ParserComponent* newParent)                         override;
    /// Adds a new composite object. This method is avalaible here to provide
    /// compatibility with interface. Here its empty
    virtual void                        addChild(uint8_t cmd , QSharedPointer<interface::ParserComponent> child) override;
    /// @return This method returns false if this object is a leaf.
    virtual bool                        isComposite()                                                            const override;
    /// Sets a new buffer from which bytes are pulled
    /// @param newBuffer A new buffer
    virtual void                        setBuffer(interface::ByteBuffer* newBuffer)                              override;
protected:
    /// Allocates a new package
    virtual void                        createPackage()                                                          override;
};

/// @brief This class provides composite objects which can have children.
///
/// Also complex parsers have processing loop because they are not the last stage
/// in the processing tree, leafs are the last stages.
/// These object forwards its work to the children recursively.
class ComplexParser:
        public ParserComponent,
        public global::ProgramObject
{
protected:
    /// Children of the composite object binded to the certain command
    QMap<Code , QSharedPointer<interface::ParserComponent>> children_;
    /// This variable guards commands enum range. For example backend::GlobalCommand enum
    /// has 3 values ,so value of this variable equals 3. It is used to
    /// data flow error checking by checkCode() function.
    Code                                                    commands_count_;
public:
    /// Default constructor.
    /// @param Commands count in a certain enum
    ComplexParser(Code commands_count);
    /// @return This method returns false if this object is a leaf.
    virtual bool isComposite()                                                            const override;
    /// Adds a new composite object. Bind a new composite parser object to the
    /// corresponding command.
    /// @param cmd Command
    /// @param child Composite parser which parses this command
    virtual void addChild(uint8_t cmd , QSharedPointer<interface::ParserComponent> child) override;
    /// Sets a new buffer from which bytes are pulled. It also bind its children
    /// to this buffer
    /// @param newBuffer A new buffer
    virtual void setBuffer(interface::ByteBuffer* newBuffer)                              override;
protected:
    /// Check correctness of the incoming command. If flow error has occurred , throw
    /// @param cmd Command to check
    /// @param class_name Composite object name where cmd is checking
    void         checkCode(Code cmd, std::string class_name);
    /// This loop pulls byte from buffer , checks it and redirects it to
    /// a child parseCommand() method
    /// @see commands_count_
    void         proccessingLoop();
    /// This is template method which is essential part of the proccesing tree.
    /// There are invoked all functions/methods which complex parsers requires.
    /// Complex parsers can customize this template method by overriding the following methods:
    /// - specialPreOperations()
    /// - specialPostOperations()
    /// - createPackage()
    /// @return Always returns true
    virtual bool parseCommand(AcceptedTypes result)                                       override;
    /// Special operations before proccessingLoop()
    /// @param result Parser's package , so it can be modified here
    virtual void specialPreOperations(AcceptedTypes result);
    /// Special operations after proccessingLoop()
    /// /// @param result Parser's package , so it can be modified here
    virtual void specialPostOperations(AcceptedTypes result);
};

/// @brief Root of the processing tree.
///
/// It receives commands from buffer and processes it.
/// Recognizes commands from GlobalCommand enum type. Forwards work
/// recursively further to the children. After finished processing
/// it can return smart pointer to entire parsed transaction package
class RootParser:
        public ComplexParser
{
public:
    ///Default constructor
    RootParser();
    ///Creates a new transaction
    virtual void               createPackage()   override;
    /// @return Returns entire parsed transaction
    virtual TransactionPackPtr getPackage()      override;
    /// Starts parsing procedure for the entire tree
    virtual void               startProcessing() override;
};

/// @brief This parser recognizes start signal and returns true
class GlobalStartParser:
        public ParserComponent
{
public:
    virtual bool parseCommand(AcceptedTypes result) override;
};

/// @brief If certain command is not supported , return only true
using EmptyParser = GlobalStartParser;

/// @brief Returns false ,so parsing of all package is finished
class EndParser:
        public ParserComponent
{
protected:
    virtual bool parseCommand(AcceptedTypes result) override;
};

///  @brief Parses all test case.
/// It receives commands from buffer and processes it.
/// Recognizes commands from TestCaseCommand enum type.
class TestCaseParser:
        public ComplexParser
{
public:
    /// Default constructor
    TestCaseParser();
protected:
    /// Craetes a test case
    virtual void createPackage()                            override;
    /// This method aquires a test case name from the buffer. Also
    /// appends a new test case
    /// @param result Transaction to which test case is appended
    virtual void specialPreOperations(AcceptedTypes result) override;
};

///  @brief Parses all unit test
/// It receives commands from buffer and processes it.
/// Recognizes commands from UnitTestCommand enum type.
class UnitTestParser:
        public ComplexParser
{
public:
    UnitTestParser();
protected:
    /// Appends a new unit test
    /// @param result Test case to which unit test is appended
    virtual void specialPreOperations(AcceptedTypes result) override;
};

///  @brief Parses unit test expression name.
class NameParser:
        public ParserComponent
{
protected:
    /// Writes unit test expression name to a unit test
    /// @param result Unit test to which name is written
    virtual bool parseCommand(AcceptedTypes result) override;
};

///  @brief Parses unit test type descriptor.
class TypeDescriptorParser:
        public ParserComponent
{
protected:
    /// Reads type descriptor from the buffer and writes it
    /// in to current_type_ static variable.
    /// @param result Unit test to which type descriptor is written
    virtual bool parseCommand(AcceptedTypes result) override;
};

/// @brief Base class for parsers which parses a numeric value
///
/// Numeric values are written in several bytes ,so they have to
/// know size of it. This size can be read from current_type_
/// static value.
/// Derived classes can only overload redirectValueBytes() method
/// to redirect a number to corresponding unit test package variable
class ValueParser:
        public ParserComponent
{
protected:
    /// Collects bytes of a number from the buffer and redirects them
    /// to corresponding unit test package variable
    /// @param result Unit test which aquire this number
    virtual bool parseCommand(AcceptedTypes result)                 override final;
    /// This method must be overrided by dervied classes.
    /// @param value_result Bytes of a number
    /// @param unit_test Test to which write bytes of a number
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test )    = 0;
};

/// @brief Redirects number's bytes to unit test current value
class CurrentValueParser:
        public ValueParser
{
protected:
    /// Redirects number's bytes to unit test current value
    /// @param value_result Bytes of a number
    /// @param unit_test Test to which write bytes of a number
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test)     override;
};

/// @brief Redirects number's bytes to unit test expected value
class ExpectedValueParser:
        public ValueParser
{
protected:
    /// Redirects number's bytes to unit test expected value
    /// @param value_result Bytes of a number
    /// @param unit_test Test to which write bytes of a number
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test)     override;
};

/// @brief Redirects number's bytes to unit test lower value
class LowerValueParser:
        public ValueParser
{
protected:
    /// Redirects number's bytes to unit test lower value
    /// @param value_result Bytes of a number
    /// @param unit_test Test to which write bytes of a number
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test)     override;
};

/// @brief Redirects number's bytes to unit test upper value
class UpperValueParser:
        public ValueParser
{
protected:
    /// Redirects number's bytes to unit test upper value
    /// @param value_result Bytes of a number
    /// @param unit_test Test to which write bytes of a number
    virtual void redirectValueBytes(const QByteArray& value_result,
                                    UnitTestPackPtr& unit_test)     override;
};

/// @brief This parser reads unit test result from the buffer
class TestResultParser:
        public ParserComponent
{
protected:
    /// Reads a single byte which describes test result.
    /// @param result Unit test to which test result is written
    virtual bool parseCommand(AcceptedTypes result) override;
};

/// This type provides enumerations for aquring implementantions
/// from backend::ReadyParsers static generator
enum class ParserImplementations
{
    FirstImplementation /// Implementation for v1.0
};

/// It stores ready parsers tree implementations and provides pointers to them
using ReadyParsers  = global::StaticGenerator< ParserImplementations , QSharedPointer<RootParser>>;

}

namespace global
{

template<>
void backend::ReadyParsers::initValues();

}
