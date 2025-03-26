/*
3. Create a class, CMyStack, to save char elements and implement the operations
on the stack as follow members:

    class CMyStack
    {
    private:
        char *m_pTop; // Top pointer of stack
        int m_iSize; // Number of actual elements
        int m_iCapacity; // Capacity of stack
    public:
        CMyStack(int size);
        ~CMyStack();
        char Pop();
        char Peek();
        bool Push(char ch);
        bool isEmpty(); // Is Stack empty?
        bool isFull(); // Is Stack full?
        int GetSize(); // Get number of actual elements
        void Clear(); // Clear stack
    };

Note: Don't modify any member variables and interface of member function in
CMyStack.
*/

/*
4. Create a class , CExpression to calculate the value of an expression which consists of numbers and operators, such as + - * / and ( )
    Define member functions such as following:

    class CExpression
    {
    private:
        //
    public:
        double Value( ); // Get value of expresstion
        ostream& operator << (ostream& os, const CExpression& expr); // print only expression except its value
        //
    };

NOTE:
    4.1 You can define appropriate member functions and variables.
    4.2 You MUST use CMyStack you have finished to complete the program together.
    4.3 Assume that an expression you input is always correct, that is , there is no grammatical errors.

CExpression can be used in the following way in the main:
    int main()
    {
        CExpression expr("50.3-20.12+8*8/2");
        cout << expr << " = " << expr.Value() << endl; // 50.3-20.12+8*8/2 = 62.18

        expr.SetExpression("55.99-88.11+77.12");
        cout << expr << " = " << expr.Value() << endl; // 55.99-88.11+77.12 = 45

        expr.SetExpression("(39+11)*30+10/5");
        cout << expr << " = " << expr.Value() << endl; // (39+11)*30+10/5 = 1502

        expr.SetExpression("39+12*(47+33)");
        cout << expr << " = " << expr.Value() << endl; // 39+12*(47+33) = 999

        expr.SetExpression("20/(112-(10*1.2))/10-1.01");
        cout << expr << " = " << expr.Value() << endl; // 20/(112-(10*1.2))/10-1.01 = -0.99

        cout << "ENDING..." << endl;
        return 0;
    }
*/

#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <cctype>
#include <sstream>

class CMyStack
{
private:
    char *m_pTop;      // Top pointer of stack
    int m_iSize;       // Number of actual elements
    int m_iCapacity;   // Capacity of stack
    
public:
    CMyStack(int size);
    ~CMyStack();
    char Pop();
    char Peek();
    bool Push(char ch);
    bool isEmpty();    // Is Stack empty?
    bool isFull();     // Is Stack full?
    int GetSize();     // Get number of actual elements
    void Clear();      // Clear stack
};

class CExpression
{
private:
    std::string m_strExpression;  // The expression to evaluate
    
    // Helper methods for expression evaluation

    /**
     * Evaluate the postfix expression
     * @param postfix the postfix expression to evaluate
     * @return the value of the postfix expression
     */
    double evaluatePostfix(const std::string& postfix);

    /**
     * Convert the infix expression to postfix notation
     * @param infix the infix expression to convert
     * @return the postfix expression
     */
    std::string infixToPostfix(const std::string& infix);

    /**
     * Check if a character is numeric
     * @param c the character to check
     * @return true if the character is numeric, false otherwise
     */
    bool isNumeric(char c) const;

    /**
     * Check if a character is an operator
     * @param c the character to check
     * @return true if the character is an operator, false otherwise
     */
    bool isOperator(char c) const;

    /**
     * Get the precedence of an operator
     * @param op the operator to get the precedence of
     * @return the precedence of the operator
     */
    int precedence(char op) const;
    
public:
    /**
     * Constructor with optional initial expression
     * @param expression the initial expression to set
     */
    CExpression(const std::string& expression = "");
    
    double Value(); // Get the value of the expression
    
    /**
     * Set a new expression
     * @param expression the new expression to set
     */
    void SetExpression(const std::string& expression);
    
    /**
     * Get the expression string
     * @return the expression string
     */
    std::string GetExpression() const;
    
    friend std::ostream& operator<<(std::ostream& os, const CExpression& expr); // print only expression except its value
};

// Implementation of CMyStack methods

// Constructor - initializes the stack with given capacity
CMyStack::CMyStack(int size)
{
    if (size <= 0)
    {
        throw std::invalid_argument("Stack size must be positive");
    }
    
    m_pTop = new char[size];
    if (m_pTop == nullptr)
    {
        throw std::bad_alloc();
    }
    
    m_iCapacity = size;
    m_iSize = 0;
}

// Destructor - releases allocated memory
CMyStack::~CMyStack()
{
    delete[] m_pTop;
    m_pTop = nullptr;
    m_iSize = 0;
    m_iCapacity = 0;
}

// Pop the top element from the stack
char CMyStack::Pop()
{
    if (isEmpty())
    {
        throw std::underflow_error("Stack is empty");
    }
    
    return m_pTop[--m_iSize];
}

// Peek at the top element without removing it
char CMyStack::Peek()
{
    if (isEmpty())
    {
        throw std::underflow_error("Stack is empty");
    }
    
    return m_pTop[m_iSize - 1];
}

// Push a character onto the stack
bool CMyStack::Push(char ch)
{
    if (isFull())
    {
        return false;  // Return false instead of throwing exception
    }
    
    m_pTop[m_iSize++] = ch;
    return true;
}

// Check if the stack is empty
bool CMyStack::isEmpty()
{
    return m_iSize <= 0;
}

// Check if the stack is full
bool CMyStack::isFull()
{
    return m_iSize >= m_iCapacity;
}

// Get the current number of elements in the stack
int CMyStack::GetSize()
{
    return m_iSize;
}

// Clear all elements from the stack
void CMyStack::Clear()
{
    m_iSize = 0;
}

// Implementation of CExpression methods

// Constructor with optional initial expression
CExpression::CExpression(const std::string& expression)
    : m_strExpression(expression)
{
}

// Set a new expression
void CExpression::SetExpression(const std::string& expression)
{
    m_strExpression = expression;
}

// Get the expression string
std::string CExpression::GetExpression() const
{
    return m_strExpression;
}

// Check if a character is numeric (digit or decimal point)
bool CExpression::isNumeric(char c) const
{
    return std::isdigit(c) || c == '.';
}

// Check if a character is an operator
bool CExpression::isOperator(char c) const
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Get the precedence of an operator
int CExpression::precedence(char op) const
{
    if (op == '+' || op == '-')
    {
        return 1;
    }
    else if (op == '*' || op == '/')
    {
        return 2;
    }
    return 0;
}

// Convert infix expression to postfix notation
std::string CExpression::infixToPostfix(const std::string& infix)
{
    std::string postfix;
    CMyStack stack(infix.length());  // Stack for operators
    
    for (size_t i = 0; i < infix.length(); i++)
    {
        char c = infix[i];
        
        // If character is numeric, collect the full number
        if (isNumeric(c))
        {
            // Extract the complete number
            while (i < infix.length() && (isNumeric(infix[i])))
            {
                postfix += infix[i++];
            }
            i--; // Adjust for the loop increment
            
            // Add space as delimiter
            postfix += ' ';
        }
        // If character is an opening parenthesis, push to stack
        else if (c == '(')
        {
            stack.Push(c);
        }
        // If character is a closing parenthesis, pop until matching opening parenthesis
        else if (c == ')')
        {
            while (!stack.isEmpty() && stack.Peek() != '(')
            {
                postfix += stack.Pop();
                postfix += ' ';
            }
            
            // Pop the opening parenthesis
            if (!stack.isEmpty() && stack.Peek() == '(')
            {
                stack.Pop();
            }
        }
        // If character is an operator
        else if (isOperator(c))
        {
            // Pop operators with higher or equal precedence
            while (!stack.isEmpty() && stack.Peek() != '(' && 
                   precedence(stack.Peek()) >= precedence(c))
            {
                postfix += stack.Pop();
                postfix += ' ';
            }
            
            // Push current operator
            stack.Push(c);
        }
    }
    
    // Pop remaining operators
    while (!stack.isEmpty())
    {
        if (stack.Peek() != '(')
        {
            postfix += stack.Pop();
            postfix += ' ';
        }
        else
        {
            stack.Pop();
        }
    }
    
    return postfix;
}

// Evaluate a postfix expression
double CExpression::evaluatePostfix(const std::string& postfix)
{
    CMyStack stack(postfix.length() * 2);  // Double the stack size for safety
    std::stringstream ss(postfix);
    std::string token;
    
    while (ss >> token)
    {
        // If token is a number
        if (isNumeric(token[0]) || (token.length() > 1 && token[0] == '-' && isNumeric(token[1])))
        {
            // Push the token directly onto the stack character by character
            for (char c : token)
            {
                if (!stack.Push(c))  // Check if push was successful
                {
                    throw std::runtime_error("Stack overflow");
                }
            }
            if (!stack.Push('#'))  // Check if push was successful
            {
                throw std::runtime_error("Stack overflow");
            }
        }
        // If token is an operator
        else if (token.length() == 1 && isOperator(token[0]))
        {
            // Extract second operand
            std::string operand2Str;
            if (stack.isEmpty())
            {
                throw std::runtime_error("Stack underflow");
            }
            stack.Pop(); // Pop the '#' delimiter
            while (!stack.isEmpty() && stack.Peek() != '#')
            {
                operand2Str = stack.Pop() + operand2Str;
            }
            
            // Extract first operand
            std::string operand1Str;
            if (stack.isEmpty())
            {
                throw std::runtime_error("Stack underflow");
            }
            stack.Pop(); // Pop the '#' delimiter
            while (!stack.isEmpty() && stack.Peek() != '#')
            {
                operand1Str = stack.Pop() + operand1Str;
            }
            
            // Convert strings to doubles
            double operand1 = std::stod(operand1Str);
            double operand2 = std::stod(operand2Str);
            
            // Perform operation
            double result = 0.0;
            switch (token[0])
            {
                case '+': result = operand1 + operand2; break;
                case '-': result = operand1 - operand2; break;
                case '*': result = operand1 * operand2; break;
                case '/': result = operand1 / operand2; break;
                default: throw std::runtime_error("Invalid operator");
            }
            
            // Format the result with limited precision to avoid stack overflow
            std::stringstream resultStream;
            resultStream.precision(6);  // Limit to 6 decimal places
            resultStream << std::fixed << result;
            std::string resultStr = resultStream.str();
            
            // Remove trailing zeros to save space
            size_t decimalPos = resultStr.find('.');
            if (decimalPos != std::string::npos)
            {
                // Trim trailing zeros
                size_t lastNonZero = resultStr.find_last_not_of('0');
                if (lastNonZero != std::string::npos && lastNonZero > decimalPos)
                {
                    resultStr.erase(lastNonZero + 1);
                }
                else if (lastNonZero == decimalPos)
                {
                    resultStr.erase(decimalPos);
                }
            }
            
            // Push result back to stack with overflow checking
            for (char c : resultStr)
            {
                if (!stack.Push(c))
                {
                    throw std::runtime_error("Stack overflow");
                }
            }
            if (!stack.Push('#'))
            {
                throw std::runtime_error("Stack overflow");
            }
        }
    }
    
    // Extract final result
    std::string resultStr;
    stack.Pop(); // Pop the '#' delimiter
    while (!stack.isEmpty() && stack.Peek() != '#')
    {
        resultStr = stack.Pop() + resultStr;
    }
    
    if (!stack.isEmpty())
    {
        throw std::runtime_error("Stack is not empty");
    }
    // Convert to double
    return std::stod(resultStr);
}

// Calculate the value of the expression
double CExpression::Value()
{
    if (m_strExpression.empty())
    {
        return 0.0;
    }
    
    try
    {
        // First convert infix to postfix notation
        std::string postfix = infixToPostfix(m_strExpression);
        
        // Then evaluate the postfix expression
        return evaluatePostfix(postfix);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error evaluating expression: " << e.what() << std::endl;
        return 0.0;
    }
}

// Stream output operator
std::ostream& operator<<(std::ostream& os, const CExpression& expr)
{
    os << expr.GetExpression();
    return os;
}

int main()
{
    CExpression expr("50.3-20.12+8*8/2");
    std::cout << expr << " = " << expr.Value() << std::endl; // 50.3-20.12+8*8/2 = 62.18

    expr.SetExpression("55.99-88.11+77.12");
    std::cout << expr << " = " << expr.Value() << std::endl; // 55.99-88.11+77.12 = 45

    expr.SetExpression("(39+11)*30+10/5");
    std::cout << expr << " = " << expr.Value() << std::endl; // (39+11)*30+10/5 = 1502

    expr.SetExpression("39+12*(47+33)");
    std::cout << expr << " = " << expr.Value() << std::endl; // 39+12*(47+33) = 999

    expr.SetExpression("20/(112-(10*1.2))/10-1.01");
    std::cout << expr << " = " << expr.Value() << std::endl; // 20/(112-(10*1.2))/10-1.01 = -0.99

    std::cout << "ENDING..." << std::endl;
    return 0;
}