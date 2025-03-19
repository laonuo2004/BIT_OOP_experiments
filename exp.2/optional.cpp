/*
Define a class of CLoopSet which holds data with linear structure and connects 
the last node to the first node, and reimplement all of the member functions 
in the CSet above.
    Note:
    (1) You may define compatible parameters as well as other members you need.
    (2) Templates in STL(Standard Template Library) of C++ are FORBIDDEN.
*/

#include <iostream>
#include <stdexcept>

// Node structure for circular linked list
struct Node 
{
    int data;       // Integer data stored in the node
    Node* next;     // Pointer to the next node
    
    /**
     * Constructor for node with value
     * @param value The integer value to store
     */
    explicit Node(int value) 
        : data(value), next(nullptr) {}
};

/**
 * Integer Set Class using Circular Linked List implementation
 * Maintains a sorted circular linked list of unique integers
 */
class CLoopSet
{
private:
    Node* head;     // Head of the circular linked list (nullptr if empty)
    int size;       // Number of elements in the set
    
    /**
     * Helper function to find a node with specific value
     * Uses the sorted nature of the list for optimization
     * @param item The value to search for
     * @return Pointer to node containing the value, or nullptr if not found
     */
    Node* findNode(int item) const;
    
    /**
     * Helper function to find a node that points to a node with specific value
     * @param item The value to search for
     * @return Pointer to node before the one containing value, or nullptr if not found
     */
    Node* findPrevNode(int item) const;
    
    /**
     * Helper function to clean up all nodes in the list
     */
    void clearList();
    
    /**
     * Helper function to copy nodes from another list
     * @param other Source list to copy from
     * @throws std::bad_alloc If memory allocation fails
     */
    void copyList(const CLoopSet& other);
    
    /**
     * Helper function to insert a new node in sorted order
     * @param newNode Pointer to new node to insert (already allocated)
     * @return true if inserted successfully
     */
    bool insertNodeSorted(Node* newNode);
    
public:
    /**
     * Default constructor - creates an empty set
     */
    CLoopSet();
    
    /**
     * Copy constructor - creates a deep copy of another set
     * @param other The set to copy from
     * @throws std::bad_alloc If memory allocation fails
     */
    CLoopSet(const CLoopSet& other);
    
    /**
     * Assignment operator - assigns contents from another set
     * @param other The set to assign from
     * @return Reference to this object
     * @throws std::bad_alloc If memory allocation fails
     */
    CLoopSet& operator=(const CLoopSet& other);
    
    /**
     * Destructor - releases all resources
     */
    ~CLoopSet();
    
    /**
     * Checks if an item exists in the set
     * @param item The item to check
     * @return true if item exists, false otherwise
     */
    bool isExist(int item) const;
    
    /**
     * Checks if two sets are equal (contain the same elements)
     * @param other The set to compare with
     * @return true if sets are equal, false otherwise
     */
    bool isEqual(const CLoopSet& other) const;
    
    /**
     * Creates a new set containing the intersection with another set
     * @param other The set to intersect with
     * @return A new set containing common elements
     * @throws std::bad_alloc If memory allocation fails
     */
    CLoopSet intersection(const CLoopSet& other) const;
    
    /**
     * Creates a new set containing the union with another set
     * @param other The set to unite with
     * @return A new set containing all elements from both sets
     * @throws std::bad_alloc If memory allocation fails
     */
    CLoopSet getUnion(const CLoopSet& other) const;
    
    /**
     * Removes an item from the set
     * @param item The item to remove
     * @return true if item was found and removed, false if item not in set
     */
    bool removeItem(int item);
    
    /**
     * Adds an item to the set if it doesn't already exist
     * @param item The item to add
     * @return true if item was added successfully, false if already exists
     * @throws std::bad_alloc If memory allocation fails
     */
    bool addItem(int item);
    
    /**
     * Gets an item at the specified position
     * @param position The position (0-based index)
     * @return The item at the position
     * @throws std::out_of_range If position is invalid
     */
    int getItem(int position) const;
    
    /**
     * Gets the current size of the set
     * @return Number of elements in the set
     */
    int getSize() const;
    
    /**
     * Clears the set, removing all elements
     */
    void clear();
    
    /**
     * Displays the set elements in order
     */
    void display() const;
    
    /**
     * Equality operator - checks if two sets contain the same elements
     * @param other The set to compare with
     * @return true if sets are equal, false otherwise
     */
    bool operator==(const CLoopSet& other) const;
    
    /**
     * Inequality operator - checks if two sets are different
     * @param other The set to compare with
     * @return true if sets are different, false otherwise
     */
    bool operator!=(const CLoopSet& other) const;
};

// Default constructor
CLoopSet::CLoopSet() : head(nullptr), size(0) {}

// Copy constructor
CLoopSet::CLoopSet(const CLoopSet& other) : head(nullptr), size(0)
{
    copyList(other);
}

// Assignment operator
CLoopSet& CLoopSet::operator=(const CLoopSet& other)
{
    if (this != &other)
    {
        clearList();
        copyList(other);
    }
    return *this;
}

// Destructor
CLoopSet::~CLoopSet()
{
    clearList();
}

// Equality operator
bool CLoopSet::operator==(const CLoopSet& other) const
{
    return isEqual(other);
}

// Inequality operator
bool CLoopSet::operator!=(const CLoopSet& other) const
{
    return !isEqual(other);
}

// Helper function to find a node with specific value
Node* CLoopSet::findNode(int item) const
{
    if (head == nullptr)
    {
        return nullptr;
    }
    
    // use sorted property for early termination
    Node* current = head;
    do
    {
        if (current->data == item)
        {
            return current;
        }
        
        // Since list is sorted, we can stop if we've gone past the value
        if (current->data > item && current == head)
        {
            // Special case: head is already larger than item
            break;
        }
        
        if (current->next != head && current->next->data > item)
        {
            // We've gone past the value without finding it
            break;
        }
        
        current = current->next;
    } while (current != head);
    
    return nullptr;
}

// Helper function to find a node that points to a specific value
Node* CLoopSet::findPrevNode(int item) const
{
    if (head == nullptr)
    {
        return nullptr;
    }
    
    // Special case: if head contains the item
    if (head->data == item)
    {
        // Find the last node that points to head
        Node* current = head;
        while (current->next != head)
        {
            current = current->next;
        }
        return current;
    }
    
    // use sorted property
    Node* current = head;
    while (current->next != head)
    {
        if (current->next->data == item)
        {
            return current;
        }
        
        if (current->next->data > item)
        {
            // We've gone past the value without finding it
            break;
        }
        
        current = current->next;
    }
    
    return nullptr;
}

// Helper function to clear all nodes
void CLoopSet::clearList()
{
    if (head == nullptr)
    {
        return;
    }
    
    Node* current = head->next;
    Node* toDelete = nullptr;
    
    // Handle all nodes except the last one which points back to head
    while (current != head)
    {
        toDelete = current;
        current = current->next;
        delete toDelete;
    }
    
    // Finally delete the head node
    delete head;
    head = nullptr;
    size = 0;
}

// Helper function to copy nodes from another list
void CLoopSet::copyList(const CLoopSet& other)
{
    if (other.head == nullptr)
    {
        return;
    }
    
    // Use a separate method to ensure exception safety
    try
    {
        // Copy all nodes one by one
        Node* srcCurrent = other.head;
        do
        {
            if (!addItem(srcCurrent->data))
            {
                // This should never happen
                throw std::runtime_error("Failed to add item during copy operation");
            }
            srcCurrent = srcCurrent->next;
        } while (srcCurrent != other.head);
    }
    catch (...)
    {
        // Clean up any partial copy
        clearList();
        throw;  // Re-throw the exception
    }
}

// Helper function to insert a new node in sorted order
bool CLoopSet::insertNodeSorted(Node* newNode)
{
    if (newNode == nullptr)
    {
        return false;
    }
    
    // Case 1: Empty list
    if (head == nullptr)
    {
        head = newNode;
        newNode->next = head;  // Point to itself
        size = 1;
        return true;
    }
    
    // Case 2: Insert at the beginning (if smaller than head)
    if (newNode->data < head->data)
    {
        // Find the last node
        Node* last = head;
        while (last->next != head)
        {
            last = last->next;
        }
        
        // Insert new node as head
        newNode->next = head;
        last->next = newNode;
        head = newNode;
        size++;
        return true;
    }
    
    // Case 3: Insert in the middle or end
    Node* current = head;
    while (current->next != head && current->next->data < newNode->data)
    {
        current = current->next;
    }
    
    // Insert after current
    newNode->next = current->next;
    current->next = newNode;
    size++;
    return true;
}

// Check if an item exists in the set
bool CLoopSet::isExist(int item) const
{
    return findNode(item) != nullptr;
}

// Check if two sets are equal
bool CLoopSet::isEqual(const CLoopSet& other) const
{
    // Quick check of sizes
    if (size != other.size)
    {
        return false;
    }
    
    // Empty sets are equal, note that we've already excluded the case where size != other.size
    if (size == 0)
    {
        return true;
    }
    
    // Since both lists are sorted, we can compare them directly
    Node* thisCurrent = head;
    Node* otherCurrent = other.head;
    
    do
    {
        if (thisCurrent->data != otherCurrent->data)
        {
            return false;
        }
        
        thisCurrent = thisCurrent->next;
        otherCurrent = otherCurrent->next;
    } while (thisCurrent != head);
    
    return true;
}

// Add an item to the set
bool CLoopSet::addItem(int item)
{
    // Don't add if the item already exists
    if (isExist(item))
    {
        return false;
    }
    
    try
    {
        Node* newNode = new Node(item);
        if (!insertNodeSorted(newNode))
        {
            delete newNode;
            return false;
        }
        return true;
    }
    catch (std::bad_alloc&)
    {
        throw;
    }
}

// Remove an item from the set
bool CLoopSet::removeItem(int item)
{
    // Can't remove from an empty list
    if (head == nullptr)
    {
        return false;
    }
    
    Node* prev = findPrevNode(item);
    if (prev == nullptr)
    {
        return false;  // Item not found
    }
    
    Node* toDelete = prev->next;
    
    // Case 1: Only one node in the list
    if (toDelete == head && toDelete->next == head)
    {
        delete head;
        head = nullptr;
        size = 0;
        return true;
    }
    
    // Case 2: Removing head node
    if (toDelete == head)
    {
        prev->next = head->next;
        head = head->next;
    }
    else
    {
        // Case 3: Removing a non-head node
        prev->next = toDelete->next;
    }
    
    delete toDelete;
    size--;
    return true;
}

// Get item at specified position
int CLoopSet::getItem(int position) const
{
    if (position < 0 || position >= size)
    {
        throw std::out_of_range("Position out of range");
    }
    
    if (head == nullptr)
    {
        throw std::logic_error("Cannot get item from empty set");
    }
    
    Node* current = head;
    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }
    
    return current->data;
}

// Get intersection with another set
CLoopSet CLoopSet::intersection(const CLoopSet& other) const
{
    CLoopSet result;
    
    // Empty result if either set is empty
    if (head == nullptr || other.head == nullptr)
    {
        return result;
    }
    
    // use sorted properties of both lists
    Node* thisCurrent = head;
    
    do
    {
        if (other.isExist(thisCurrent->data))
        {
            result.addItem(thisCurrent->data);
        }
        thisCurrent = thisCurrent->next;
    } while (thisCurrent != head);
    
    return result;
}

// Get union with another set
CLoopSet CLoopSet::getUnion(const CLoopSet& other) const
{
    CLoopSet result;
    
    // Add all elements from this set
    if (head != nullptr)
    {
        Node* current = head;
        do
        {
            result.addItem(current->data);
            current = current->next;
        } while (current != head);
    }
    
    // Add all elements from other set (duplicates will be automatically handled by addItem)
    if (other.head != nullptr)
    {
        Node* current = other.head;
        do
        {
            result.addItem(current->data);
            current = current->next;
        } while (current != other.head);
    }
    
    return result;
}

// Get the size of the set
int CLoopSet::getSize() const
{
    return size;
}

// Clear all elements from the set
void CLoopSet::clear()
{
    clearList();
}

// Display the set elements
void CLoopSet::display() const
{
    std::cout << "{ ";
    
    if (head != nullptr)
    {
        Node* current = head;
        do
        {
            std::cout << current->data;
            current = current->next;
            if (current != head)
            {
                std::cout << ", ";
            }
        } while (current != head);
    }
    
    std::cout << " }";
}

// Test suite
void runTests()
{
    std::cout << "===== Running CLoopSet Tests =====" << std::endl;
    
    // Test 1: Basic operations
    std::cout << "\nTest 1: Basic operations" << std::endl;
    CLoopSet set1;
    std::cout << "Empty set: ";
    set1.display();
    std::cout << " (size: " << set1.getSize() << ")" << std::endl;
    
    set1.addItem(5);
    set1.addItem(3);
    set1.addItem(7);
    std::cout << "After adding 5, 3, 7: ";
    set1.display();
    std::cout << " (size: " << set1.getSize() << ")" << std::endl;
    
    set1.removeItem(3);
    std::cout << "After removing 3: ";
    set1.display();
    std::cout << " (size: " << set1.getSize() << ")" << std::endl;
    
    // Test 2: Duplicate handling
    std::cout << "\nTest 2: Duplicate handling" << std::endl;
    bool added = set1.addItem(5);
    std::cout << "Adding duplicate 5: " << (added ? "Added" : "Not added") << std::endl;
    std::cout << "Current set: ";
    set1.display();
    std::cout << std::endl;
    
    // Test 3: Set operations
    std::cout << "\nTest 3: Set operations" << std::endl;
    CLoopSet set2;
    set2.addItem(7);
    set2.addItem(9);
    set2.addItem(2);
    
    std::cout << "Set 1: ";
    set1.display();
    std::cout << std::endl;
    
    std::cout << "Set 2: ";
    set2.display();
    std::cout << std::endl;
    
    CLoopSet intersection = set1.intersection(set2);
    std::cout << "Intersection: ";
    intersection.display();
    std::cout << std::endl;
    
    CLoopSet unionSet = set1.getUnion(set2);
    std::cout << "Union: ";
    unionSet.display();
    std::cout << std::endl;
    
    // Test 4: Copy and equality
    std::cout << "\nTest 4: Copy and equality" << std::endl;
    CLoopSet set3 = set1;  // Copy constructor
    std::cout << "Set 3 (copy of Set 1): ";
    set3.display();
    std::cout << std::endl;
    
    std::cout << "Set 1 == Set 3: " << (set1 == set3 ? "true" : "false") << std::endl;
    std::cout << "Set 1 == Set 2: " << (set1 == set2 ? "true" : "false") << std::endl;
    
    // Test 5: Edge cases
    std::cout << "\nTest 5: Edge cases" << std::endl;
    CLoopSet emptySet;
    CLoopSet emptySet2;
    
    std::cout << "Empty sets equal: " << (emptySet == emptySet2 ? "true" : "false") << std::endl;
    
    CLoopSet emptyIntersection = set1.intersection(emptySet);
    std::cout << "Intersection with empty set: ";
    emptyIntersection.display();
    std::cout << std::endl;
    
    CLoopSet emptyUnion = emptySet.getUnion(emptySet2);
    std::cout << "Union of empty sets: ";
    emptyUnion.display();
    std::cout << std::endl;
    
    // Test 6: Exception handling
    std::cout << "\nTest 6: Exception handling" << std::endl;
    try
    {
        std::cout << "Item at position 0 in Set 1: " << set1.getItem(0) << std::endl;
        std::cout << "Item at position 10 in Set 1 (should throw): ";
        std::cout << set1.getItem(10) << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    std::cout << "\n===== Tests Completed =====" << std::endl;
}

// Test CLoopSet class and its methods
int main()
{
    try
    {
        runTests();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}