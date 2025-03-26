/*
[optional] Create a class, CLINT, to save a big positive integer which is no more than 100 digits.
    Define a member function to achieve the sum of two big numbers such as following:

    class CLINT
    {
    private:
        //
    public:
    CLINT operator+ (const CLINT& L); // Achieve the sum of two big numbers
        //
    };

    CLINT can be used in the following way in the main:

    int main()
    {
        CLINT L1("12345678900987654321"), L2("9876543210"), L3;
        L3 = L1 + L2;
        cout << L3 << endl; // 12345678910864197531
        
        return 0;
    }

NOTES: You can define appropriate member functions and variables.
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

class CLINT
{
private:
    std::string m_value; // Store number as string

public:
    CLINT(); // Default constructor
    CLINT(const std::string& str); // Constructor from string
    CLINT(const CLINT& other); // Copy constructor

    /**
     * @brief Achieve the sum of two big numbers
     * @param L The CLINT object to add
     * @return A new CLINT object representing the sum of the two operands
     */
    CLINT operator+(const CLINT& L);

    /**
     * @brief Assignment operator
     * @param L The CLINT object to assign from
     * @return Reference to this object
     */
    CLINT& operator=(const CLINT& L);

    /**
     * @brief Convert the CLINT object to a string
     * @return A string representation of the CLINT object
     */
    std::string toString() const;

    /**
     * @brief Output stream operator
     * @param os The output stream
     * @param L The CLINT object to output
     * @return Reference to the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const CLINT& L);
};

// Default constructor
CLINT::CLINT() : m_value("0")
{
}

// Constructor from string
CLINT::CLINT(const std::string& str) : m_value(str)
{
    // Validate input: should only contain digits
    for (char c : m_value)
    {
        if (!std::isdigit(c))
        {
            throw std::invalid_argument("Invalid character in CLINT constructor");
        }
    }

    // Remove leading zeros
    size_t firstNonZero = m_value.find_first_not_of('0');
    if (firstNonZero != std::string::npos)
    {
        m_value = m_value.substr(firstNonZero);
    }
    else
    {
        // If all zeros, keep one zero
        m_value = "0";
    }

    // Check size constraint (100 digits maximum)
    if (m_value.length() > 100)
    {
        throw std::overflow_error("Number exceeds 100 digits limit");
    }
}

// Copy constructor
CLINT::CLINT(const CLINT& other) : m_value(other.m_value)
{
}

// Assignment operator
CLINT& CLINT::operator=(const CLINT& L)
{
    if (this != &L)
    {
        m_value = L.m_value;
    }
    return *this;
}

// Achieve the sum of two big numbers
CLINT CLINT::operator+(const CLINT& L)
{
    CLINT result;
    std::string sum;

    // Get references to both number strings
    const std::string& a = m_value;
    const std::string& b = L.m_value;

    // Start from the end (least significant digit)
    int i = a.length() - 1;
    int j = b.length() - 1;
    int carry = 0;

    // Add digits from right to left
    while (i >= 0 || j >= 0 || carry > 0)
    {
        int digit_a = (i >= 0) ? (a[i] - '0') : 0;
        int digit_b = (j >= 0) ? (b[j] - '0') : 0;
        
        int digit_sum = digit_a + digit_b + carry;
        carry = digit_sum / 10;
        sum.push_back('0' + (digit_sum % 10));
        
        i--;
        j--;
    }

    if (m_value.length() > 100)
    {
        throw std::overflow_error("Number exceeds 100 digits limit");
    }

    // Reverse the sum (since we added from right to left)
    std::reverse(sum.begin(), sum.end());
    result.m_value = sum;

    return result;
}

// Convert to string
std::string CLINT::toString() const
{
    return m_value;
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const CLINT& L)
{
    os << L.toString();
    return os;
}

int main()
{
    CLINT L1("12345678900987654321"), L2("9876543210"), L3;
    L3 = L1 + L2;
    std::cout << L3 << std::endl; // 12345678910864197531
    
    return 0;
}