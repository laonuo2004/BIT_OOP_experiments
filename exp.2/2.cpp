/*
2. Define a class of CSmart which can print how many objects of CSmart there are in the program, and explain the results of the procedure.
    NOTE: Don't modify any codes EXCEPT CSmart class.
    class CSmart
    {
        Here is your definition ...
    };
    void DoSomething()
    {
        CSmart s;
    }

    CSmart s1;

    int main() {
        CSmart s2;
        DoSomething();
        CSmart *s3 = new CSmart;
        delete s3;
        s2.~CSmart();
        return 0;
    }
    The outputs of main are as below:
        1 object
        2 objects
        3 objects
        2 objects
        3 objects
        2 objects
        1 object
        0 object
*/

#include <iostream>

class CSmart
{
private:
    static int instanceCount;  // Static counter to track instances
    bool isDestroyed;          // Flag to prevent double destruction counting

public:
    /**
     * Constructor - increments counter and prints current count
     */
    CSmart();
    
    /**
     * Destructor - decrements counter and prints current count
     */
    ~CSmart();
    
    /**
     * Helper function to print the current instance count
     */
    static void printCount();
};

// Initialize static member
int CSmart::instanceCount = 0;

// Constructor
CSmart::CSmart()
{
    // Initialize destruction flag
    isDestroyed = false;

    // Increment the counter and print
    instanceCount++;
    printCount();
}

// Destructor
CSmart::~CSmart()
{
    // Only decrement count if not already destroyed
    if (!isDestroyed)
    {
        isDestroyed = true;
        instanceCount--;
        printCount();
    }
}

// Helper function to print count with proper grammar
void CSmart::printCount()
{
    if (instanceCount < 0)
    {
        std::cout << "Error: Negative instance count" << std::endl;
        return;
    }

    if (instanceCount == 0 || instanceCount == 1)
    {
        std::cout << instanceCount << " object" << std::endl;
    }
    else
    {
        std::cout << instanceCount << " objects" << std::endl;
    }
}

void DoSomething()
{
    CSmart s;
}

CSmart s1;

int main() 
{
    CSmart s2;
    DoSomething();
    CSmart *s3 = new CSmart;
    delete s3;
    s2.~CSmart();
    return 0;
}
/*
The outputs of main are as below:
    1 object  : s1 is created in global scope
    2 objects : s2 is created in main
    3 objects : temporary object s is created in DoSomething
    2 objects : s is destroyed when DoSomething ends
    3 objects : s3 is created in main
    2 objects : s3 is deleted
    1 object  : s2 is destroyed (flag is set to true, but not actually destroyed)
    0 object  : s1 is destroyed (global object)
*/