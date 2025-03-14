// 1. Complete the Stash in chapter 4 according to the following demands:
//    1.1 Define a member function, Remove, to delete an element at specified position;
//    1.2 Define a member function, Contract, to release the memory when necessary

#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>

using namespace std;

class Stash
{
private:
    int size;      // Size of each space
    int quantity;  // Number of storage spaces
    int next;      // Next empty space
    // Dynamically allocated array of bytes:
    unsigned char* storage;
    
    int increment() // Dynamically increase the size of the array by doubling it
    {
        return (quantity == 0) ? 1 : quantity;
    }
    
public:
    // The rule of five:
    // 1. Destructor
    // 2. Copy constructor
    // 3. Copy assignment operator
    // 4. Move constructor
    // 5. Move assignment operator
    Stash(int sz, int initialCapacity = 16); // Constructor
    ~Stash(); // Destructor
    Stash(const Stash& other); // Copy constructor
    Stash& operator=(const Stash& other); // Copy assignment operator
    Stash(Stash&& other) noexcept; // Move constructor
    Stash& operator=(Stash&& other) noexcept; // Move assignment operator

    // Member functions
    void stashInfo() const; // Print information about the Stash
    void cleanUp(); // Clean up the Stash
    int add(const void* element); // Add an element
    void* fetch(int index); // Fetch an element
    int count() const { return next; } // Count the number of elements
    void inflate(int increase); // Increase the size of the array
    void remove(int index); // Remove an element at specified position
    void contract(double threshold = 0.3); // Release memory when necessary
}; // Stash

// Constructor
Stash::Stash(int sz, int initialCapacity) : 
    size(sz), quantity(initialCapacity), next(0)
{
    try 
    {
        storage = new unsigned char[sz * quantity];
        // Initialize storage to zero:
        memset(storage, 0, size * quantity);
        cout << "Stash created with size: " << size << ", quantity: " << quantity << endl;
    }
    catch(std::bad_alloc&) 
    {
        std::cerr << "Memory allocation failed in Stash constructor" << std::endl;
        storage = nullptr;
        quantity = 0;
        throw;
    }
}

// Destructor
Stash::~Stash()
{
    delete[] storage;
}

// Copy constructor
Stash::Stash(const Stash& other) : 
    size(other.size), quantity(other.quantity), next(other.next)
{
    try 
    {
        storage = new unsigned char[size * quantity];
        memcpy(storage, other.storage, size * next);
    }
    catch(std::bad_alloc&) 
    {
        std::cerr << "Memory allocation failed in copy constructor" << std::endl;
        storage = nullptr;
        quantity = 0;
        next = 0;
        throw;
    }
}

// Copy assignment operator
Stash& Stash::operator=(const Stash& other)
{
    if (this != &other)
    {
        try 
        {
            // Create new storage first
            unsigned char* newStorage = new unsigned char[other.size * other.quantity];
            memcpy(newStorage, other.storage, other.size * other.next);
            
            // Delete old storage
            delete[] storage;
            
            // Update member variables
            size = other.size;
            quantity = other.quantity;
            next = other.next;
            storage = newStorage;
        }
        catch(std::bad_alloc&) 
        {
            std::cerr << "Memory allocation failed in assignment operator" << std::endl;
            throw;
        }
    }
    return *this;
}

// Move constructor
Stash::Stash(Stash&& other) noexcept : 
    size(other.size), quantity(other.quantity), next(other.next), storage(other.storage)
{
    other.storage = nullptr; // Prevent deletion in destructor
    other.quantity = 0;
    other.next = 0;
    other.size = 0;
}

// Move assignment operator
Stash& Stash::operator=(Stash&& other) noexcept
{
    if (this != &other)
    {
        // Release current resources
        delete[] storage;
        
        // Transfer ownership
        size = other.size;
        quantity = other.quantity;
        next = other.next;
        storage = other.storage;
        
        // Reset source object
        other.storage = nullptr;
        other.size = 0;
        other.quantity = 0;
        other.next = 0;
    }
    return *this;
}

// Print information about the Stash
void Stash::stashInfo() const
{
    for(int i = 0; i < 50; ++i)
    {
        cout << "-";
    }
    cout << endl;
    cout << "Stash Info: " << endl;
    cout << "Size: " << size << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Next: " << next << endl;
    cout << "Storage address: ";
    if (storage == nullptr)
    {
        cout << "nullptr";
    }
    else
    {
        cout << static_cast<void*>(storage);
    }
    cout << endl;
    cout << "Storage contents: ";
    for (int i = 0; i < next; ++i)
    {
        cout << *(int*)(storage + i * size) << " ";
    }
    cout << endl;
    for(int i = 0; i < 50; ++i)
    {
        cout << "-";
    }
    cout << endl;
}

// Clean up the Stash
void Stash::cleanUp()
{
    if (storage != nullptr)
    {
        cout << "Cleaning up Stash..." << endl;
        delete[] storage;
        storage = nullptr;
        next = 0;
        quantity = 0;
        cout << "Stash cleaned up." << endl;
    }
}

// Add an element
int Stash::add(const void* element)
{
    if (next >= quantity) // Need more space?
    {
        inflate(increment());
    }
    
    // Copy element into storage at next empty space
    int startBytes = next * size;
    unsigned char* e = (unsigned char*)element;
    
    memcpy(storage + startBytes, e, size);
    
    ++next;
    return (next - 1); // Return index of added element
}

// Fetch an element
void* Stash::fetch(int index)
{
    if (index < 0 || index >= next) // Check range
    {
        throw out_of_range("Stash::fetch(): Index out of range");
    }
    return &(storage[index * size]);
}

// Increase the size of the array
void Stash::inflate(int increase)
{
    if (increase <= 0) 
    {
        return; // No increase needed
    }
    
    int newQuantity = quantity + increase;
    try
    {
        unsigned char* newStorage = new unsigned char[size * newQuantity];
        
        // Copy existing elements
        if (storage != nullptr)
        {
            memcpy(newStorage, storage, size * next);
        }
        
        // Initialize new storage to zero
        memset(newStorage + (size * next), 0, size * (newQuantity - next));
        
        // Replace old storage
        delete[] storage;
        storage = newStorage;
        quantity = newQuantity;
        
        cout << "Memory inflated. New quantity: " << quantity << endl;
    }
    catch(std::bad_alloc&)
    {
        std::cerr << "Memory allocation failed in inflate()" << std::endl;
        throw;
    }
}

// Remove an element at specified position
void Stash::remove(int index)
{
    if (index < 0 || index >= next) // Check range
    {
        throw out_of_range("Stash::remove(): Index out of range");
    }
    
    // Calculate bytes to move down
    int bytesToMove = (next - index - 1) * size;
    if (bytesToMove > 0)
    {
        memmove(
            &storage[index * size], 
            &storage[(index + 1) * size], 
            bytesToMove
        );
    }
    
    --next; // Decrease count
    cout << "Element at index " << index << " removed. New count: " << next << endl;
    
    // Consider contracting if storage utilization is low
    if (next < quantity * 0.3 && quantity > 16) 
    {
        contract();
    }
}

// Contract the storage
void Stash::contract(double threshold)
{
    if (threshold < 0.0 || threshold > 1.0)
    {
        throw invalid_argument("Stash::contract(): Threshold must be between 0 and 1");
    }
    
    // Avoid contractions for small containers or when not needed
    if (quantity <= 16 || next >= quantity * threshold)
    {
        return;
    }
    
    // Calculate new size, ensuring it can hold existing elements
    int newQuantity = max(16, next * 2);
    if (newQuantity >= quantity)
    {
        return; // No need to contract
    }
    
    try
    {
        unsigned char* newStorage = new unsigned char[size * newQuantity];
        memcpy(newStorage, storage, size * next);
        
        delete[] storage;
        storage = newStorage;
        quantity = newQuantity;
        
        cout << "Memory contracted. New quantity: " << quantity << endl;
    }
    catch(std::bad_alloc&)
    {
        std::cerr << "Memory allocation failed in contract()" << std::endl;
        // Contract failure is not critical, continue with original storage
    }
}

// Test the Stash class
int main()
{
    // Create a Stash for integers
    Stash stash(sizeof(int));
    
    // Add some integers
    for (int i = 0; i < 10; ++i)
    {
        stash.add(&i);
    }
    stash.stashInfo();
    
    // Fetch and print elements
    for (int i = 0; i < stash.count(); ++i)
    {
        int* value = static_cast<int*>(stash.fetch(i));
        cout << "Element at index " << i << ": " << *value << endl;
    }
    
    // Add more elements
    for (int i = 10; i < 100; ++i)
    {
        stash.add(&i);
    }
    stash.stashInfo();

    // Randomly remove about 80% of elements
    for(int i = 0; i < stash.count(); ++i)
    {
        if(rand() % 100 < 80)
        {
            stash.remove(i);
            --i; // Adjust index after removal
        }
    }   
    stash.stashInfo();

    // Clean up
    stash.cleanUp();
    stash.stashInfo();

    // Attempt to fetch after cleanup
    try
    {
        stash.fetch(0);
    }
    catch (const out_of_range& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    // Attempt to remove after cleanup
    try
    {
        stash.remove(0);
    }
    catch (const out_of_range& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}