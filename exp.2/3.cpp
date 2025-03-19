/*
3. Create a class, CIntChar, to archieve an integer to save a string which length is no more than 4.
Suppose that a character length is 1 byte.

    For example: "Love". It's binary form associated with an integer is：
        0100 1100 0110 1111 0111 0110 0110 0101
    (1) If the string's length is less than 4 characters, the remaining part is made up by zero. For example, "Hi" is: 0100 1000 0110 1001 0000 0000 0000 0000
    (2) If the string's length is more than 4 characters, only the first 4 characters are saved in CIntChar.

    NOTES:
        (1) You MUST define an integer int class to store a string and other appropriate members;
        (2) The string you entered is prohitated from storing in the CIntChar;
        (3) In the main, cllient may call member functions in the following way.
        
    void main()
    {
        CIntChar IC("Love");
        IC.ASC_Print(); // Print the content with string format: Love
        IC.Binary_Print(); // Print the content with binary format:
        // 0100 1100 0110 1111 0111 0110 0110 0101
        IC.Int_Print(); // Print the content with integer format: 1282373221
        cout << IC.At(3) << endl; // Get the fourth chararacter in the integer: 'v'
        cout << IC.str( ) <<endl; // Get the string: "Love"
    }
*/

#include <iostream>
#include <string>
#include <bitset>
#include <stdexcept>
#include <iomanip>

class CIntChar
{
private:
    // Constants defining the structure limits
    static const int MAX_CHARS = 4;
    static const int BITS_PER_CHAR = 8;
    
    /**
     * The integer used to store character data
     * Bits 0-7:    4th character (least significant byte)
     * Bits 8-15:   3rd character
     * Bits 16-23:  2nd character
     * Bits 24-31:  1st character (most significant byte)
     */
    int data;

public:
    /**
     * Constructor that takes a string and stores up to the first 4 characters in the integer
     * @param str The input string to be stored
     */
    CIntChar(const std::string& str);
    
    /**
     * Prints the string representation of the stored data
     */
    void ASC_Print() const;
    
    /**
     * Prints the binary representation of the stored data
     * Format: 4 bits + space + 4 bits (1 char) + space + (the same for the rest)
     * Example: 0100 1100 0110 1111 0111 0110 0110 0101(for "Love")
     * Note: The last character will not have a trailing space
     */
    void Binary_Print() const;
    
    /**
     * Prints the integer value of the stored data
     */
    void Int_Print() const;
    
    /**
     * Returns the character at the specified position (0-3)
     * @param index The position of the character (0 for the first character)
     * @return The character at the specified position
     * @throws std::out_of_range if index is invalid
     */
    char At(int index) const;
    
    /**
     * Returns the string representation of the stored data
     * @return The string representation without null characters
     */
    std::string str() const;
};

// Constructor
CIntChar::CIntChar(const std::string& str)
{
    // Initialize data to zero (all null characters)
    data = 0;
    
    // Process at most MAX_CHARS characters
    for (size_t i = 0; i < MAX_CHARS && i < str.length(); i++)
    {
        // Calculate the shift amount for this character position
        // First character goes to the most significant byte
        int shiftAmount = BITS_PER_CHAR * (MAX_CHARS - 1 - i);
        
        // Use bitwise OR to set the character at the correct position
        // Cast to unsigned char to ensure proper bit representation
        data |= (static_cast<unsigned char>(str[i]) << shiftAmount);
    }
}

// Print as ASCII string
void CIntChar::ASC_Print() const
{
    std::cout << str() << std::endl;
}

// Print as binary representation with spaces between each 4-bit group
void CIntChar::Binary_Print() const
{
    for (int i = 0; i < MAX_CHARS; i++)
    {
        // Extract each byte
        int shiftAmount = BITS_PER_CHAR * (MAX_CHARS - 1 - i);
        unsigned char byte = (data >> shiftAmount) & 0xFF;
        
        // Convert to binary string with leading zeros
        std::bitset<BITS_PER_CHAR> bits(byte);
        std::string bitStr = bits.to_string();
        
        // Print first 4 bits followed by a space, then last 4 bits
        std::cout << bitStr.substr(0, 4) << " " << bitStr.substr(4, 4);
        
        // Add space between characters except after the last character
        if (i < MAX_CHARS - 1)
        {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

// Print as integer value
void CIntChar::Int_Print() const
{
    std::cout << data << std::endl;
}

// Get character at specified position with bounds checking
char CIntChar::At(int index) const
{
    if (index < 0 || index >= MAX_CHARS)
    {
        throw std::out_of_range("Index out of range in CIntChar::At");
    }
    
    // Calculate shift amount and extract the byte
    int shiftAmount = BITS_PER_CHAR * (MAX_CHARS - 1 - index);
    return static_cast<char>((data >> shiftAmount) & 0xFF);
}

// Convert to string, stopping at the first null character
std::string CIntChar::str() const
{
    std::string result;
    result.reserve(MAX_CHARS); // Pre-allocate for efficiency
    
    for (int i = 0; i < MAX_CHARS; i++)
    {
        try
        {
            char ch = At(i);
            // Stop at the first null character
            if (ch == '\0')
            {
                break;
            }
            result.push_back(ch);
        }
        catch (const std::out_of_range&)
        {
            break;
        }
    }
    
    return result;
}

/**
 * Test function to demonstrate various operations of CIntChar
 * @param testStr The string to test with
 * @param testName A descriptive name for the test
 */
void runTest(const std::string& testStr, const std::string& testName)
{
    std::cout << "\n===== Testing with '" << testName << "' =====" << std::endl;
    
    CIntChar ic(testStr);
    
    std::cout << "ASCII Print: ";
    ic.ASC_Print();
    
    std::cout << "Binary Print: ";
    ic.Binary_Print();
    
    std::cout << "Integer Print: ";
    ic.Int_Print();
    
    // Test character access
    std::cout << "Characters: ";
    for (int i = 0; i < 4; i++)
    {
        try
        {
            char c = ic.At(i);
            if (c == '\0')
            {
                std::cout << "\\0";
            }
            else
            {
                std::cout << "'" << c << "'";
            }
            
            if (i < 3)
            {
                std::cout << ", ";
            }
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "Error: " << e.what();
        }
    }
    std::cout << std::endl;
    
    std::cout << "String representation: \"" << ic.str() << "\"" << std::endl;
}

int main()
{
    try
    {
        // Test with various string lengths
        runTest("Love", "standard 4-char string");
        runTest("Hi", "shorter 2-char string");
        runTest("Hello, World!", "longer string (should truncate)");
        runTest("", "empty string");
        
        // Test exception handling with invalid index
        std::cout << "\n===== Testing exception handling =====" << std::endl;
        CIntChar ic("Test");
        try
        {
            char c = ic.At(4);  // Out of bounds
            std::cout << "This should not print: " << c << std::endl;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << "Caught expected exception: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}