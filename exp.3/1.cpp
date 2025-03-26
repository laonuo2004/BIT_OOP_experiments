/*
1. Read the codes carefully and answer the following questions.

    #include<iostream>
    using namespace std;
    class Singleton
    {
    private:
        static Singleton *s;
        Singleton() { cout << "Constructor" << endl; }
    public:
        static Singleton* GetInstance()
        {
            if (s == nullptr) s = new Singleton();
            return s;
        }
        ~Singleton()
        {
            if (s != nullptr)
            {
                delete s;
                cout << "Realease the static s." << endl;
            }
        }
    };

    Singleton* Singleton::s = nullptr;

    int main()
    {
        Singleton *ps;
        ps = Singleton::GetInstance();
        cout << ps << endl;

        return 0;
    }

1.1 Please explain the member variable s;
1.2 Please describe the meaning above the codes;
1.3 Correct errors you find if any.
*/

/*
1.1 The member variable 's' is a static pointer to a Singleton object.
    It is used to store the single instance of the Singleton class.
    Being static means it is shared among all instances of the Singleton class.
    It is initialized to nullptr and then assigned with an instance when GetInstance() is called for the first time.

1.2 The code implements the Singleton design pattern, which ensures that a class has only one instance
    and provides a global point of access to that instance. This pattern is useful when exactly one object
    is needed to coordinate actions across the system, such as a configuration manager or a connection pool.

1.3 There are two critical errors in the implementation:

    Problem 1: Destructive self-deletion in the destructor
    - The destructor attempts to delete the static pointer 's' which points to the object itself
    - This causes a double deletion problem since the object is trying to delete itself during its own destruction
    - If the destructor is called, it would trigger infinite recursion: deleting 's' calls the destructor again
    - This is undefined behavior and could crash the program if the destructor were actually called
    - The correct implementation should separate the cleanup of the singleton instance from its destruction

    Problem 2: Memory leak due to poor lifecycle management
    - No destructor is actually called in the program's execution because 'ps' is just a pointer to the singleton
    - When main() ends, the local pointer 'ps' is destroyed but the object it points to remains in memory
    - The static instance pointed to by 's' is never properly deleted throughout the program's lifetime
    - While the operating system will reclaim the memory when the program terminates, this is poor resource management
    - In long-running applications, such as servers, this type of leak would accumulate and cause serious problems
    - A better approach is to provide a dedicated cleanup method (like a static release() function)
*/

// Corrected code implementation

#include<iostream>
using namespace std;

class Singleton
{
private:
    static Singleton *s;
    Singleton() { cout << "Constructor" << endl; }

public:
    static Singleton* GetInstance()
    {
        if (s == nullptr) s = new Singleton();
        return s;
    }
    static void release(); // Release the static s
    ~Singleton();
};

Singleton* Singleton::s = nullptr;

Singleton::~Singleton()
{
    cout << "Destructor called" << endl;
}

void Singleton::release()
{
    if (s != nullptr)
    {
        delete s;
        s = nullptr; // Remember to set s to nullptr after deletion
        cout << "Released the static s." << endl;
    }
}

int main()
{
    Singleton *ps;
    ps = Singleton::GetInstance();
    cout << ps << endl;
    
    Singleton::release();
    
    return 0;
} 
