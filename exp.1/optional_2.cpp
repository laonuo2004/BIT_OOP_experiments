/*
Define an array of int. Take the starting address of that array and use static_cast to convert it into void*.
Write a function that takes a void*, a number(indicating a number of bytes), and a value(indicating the value to which each byte should be set) as arguments.
The function should set each byte in the specified range to the spcified value(0-255).
Try out the function on your array of int.
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

// Custom exception for memory operations
class MemoryException : public runtime_error 
{
public:
    explicit MemoryException(const string& message) : runtime_error(message) {}
};

class MemoryHandler
{
private:
    bool isValidInput(void* startPtr, size_t numBytes) const; // Check if input is valid
public:
    void setMemoryBytes(void* startPtr, size_t numBytes, unsigned char value); // Set memory bytes to a specified value
    void displayMemory(const void* startPtr, size_t numBytes) const; // Display memory content in hexadecimal format
};

// Check if input parameters are valid
bool MemoryHandler::isValidInput(void* startPtr, size_t numBytes) const
{
    return (startPtr != nullptr && numBytes > 0);
}

// Set memory bytes to a specified value
void MemoryHandler::setMemoryBytes(void* startPtr, size_t numBytes, unsigned char value)
{
    if (!isValidInput(startPtr, numBytes))
    {
        throw MemoryException("Invalid input parameters");
    }
    
    // Cast void* to unsigned char* to access individual bytes
    unsigned char* bytePtr = static_cast<unsigned char*>(startPtr);
    
    try 
    {
        // Set each byte to the specified value
        for (size_t i = 0; i < numBytes; ++i)
        {
            bytePtr[i] = value;
        }
    }
    catch (const exception& e) 
    {
        throw MemoryException("Failed to set memory: " + string(e.what()));
    }
}

// Display memory content in hexadecimal format
void MemoryHandler::displayMemory(const void* startPtr, size_t numBytes) const
{
    if (!isValidInput(const_cast<void*>(startPtr), numBytes))
    {
        throw MemoryException("Invalid input parameters");
    }
    
    const unsigned char* bytePtr = static_cast<const unsigned char*>(startPtr);
    
    cout << "Memory content (hex):" << endl;
    for (size_t i = 0; i < numBytes; ++i)
    {
        cout << setw(2) << setfill('0') << hex 
             << static_cast<int>(bytePtr[i]) << " ";
        
        if ((i + 1) % 16 == 0 || (i + 1) == numBytes)
            cout << endl;
    }
    cout << dec << endl;
}

int main()
{
    MemoryHandler memHandler;
    
    // Define an array of int
    const int arraySize = 8;
    int intArray[arraySize];
    for(int i = 0; i < arraySize; ++i)
    {
        intArray[i] = (i + 1) * 10; // Initialize with some values
    }
    
    try 
    {
        // Display original array values
        cout << "Original int array values:" << endl;
        for (int i = 0; i < arraySize; ++i)
        {
            cout << intArray[i] << " ";
        }
        cout << endl;
        
        // Display original memory content
        memHandler.displayMemory(intArray, arraySize * sizeof(int));
        
        // Get the starting address and convert it to void*
        void* arrayPtr = static_cast<void*>(intArray);
        
        // Test case 1: Set each byte to 0xAA (170 in decimal)
        unsigned char byteValue = 0xAA;
        try 
        {
            memHandler.setMemoryBytes(arrayPtr, arraySize * sizeof(int), byteValue);
            
            cout << "After setting all bytes to 0x" << hex << static_cast<int>(byteValue) << dec << ":" << endl;
            memHandler.displayMemory(intArray, arraySize * sizeof(int));
            
            // Display modified array values
            cout << "Modified int array values:" << endl;
            for (int i = 0; i < arraySize; ++i)
            {
                cout << intArray[i] << " ";
            }
            cout << endl;
        }
        catch (const MemoryException& e) 
        {
            cerr << "Error: " << e.what() << endl;
        }
        
        // Restore original array
        for (int i = 0; i < arraySize; ++i)
        {
            intArray[i] = (i + 1) * 10;
        }
        
        // Test case 2: Set only a portion of the array
        cout << "\nTest case 2: Setting only the first 8 bytes to 0x55" << endl;
        byteValue = 0x55;
        try 
        {
            memHandler.setMemoryBytes(arrayPtr, 8, byteValue);
            memHandler.displayMemory(intArray, arraySize * sizeof(int));
            
            cout << "Partially modified array values:" << endl;
            for (int i = 0; i < arraySize; ++i)
            {
                cout << intArray[i] << " ";
            }
            cout << endl;
        }
        catch (const MemoryException& e) 
        {
            cerr << "Error: " << e.what() << endl;
        }
        
        // Test case 3: Test with null pointer (should throw exception)
        cout << "\nTest case 3: Testing with null pointer" << endl;
        try 
        {
            memHandler.setMemoryBytes(nullptr, 5, 0xFF);
            cout << "This line should not be reached" << endl;
        }
        catch (const MemoryException& e) 
        {
            cerr << "Expected error: " << e.what() << endl;
        }
    }
    catch (const exception& e) 
    {
        cerr << "Unexpected error occurred: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}