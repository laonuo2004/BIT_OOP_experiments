/*
1 .Define an integer set class named CSet with some memeber functions as follows:
    1.1 Multiple elements of the same type can be put in a set.
    1.2 IsExist(): To judge if an integer is a member of a set or not;
    1.3 IsEqual(): To judge if two sets are equal or not;
    1.4 Intersection(): To get intersection with another set; (交集）
    1.5 Union(): To get union with another set. （并集）
    1.6 RemoveItem(): To delete an integer from the set;
    1.7 AddItem(): To add an integer to a set.In this function adds an integer successfully when this integer is NOT in the set and there are enough space to save it in the set;
    1.8 GetItem(): To get an integer according to specified position.
    NOTES:
        (1) To complement CSet class, you may define other member functions with appropriate arguments as well as member variables if you need.
        (2) Templates in STL(Standard Template Library) of C++ are FORBIDDEN.
*/

#include <iostream>
#include <stdexcept>

enum Color { RED, BLACK };

// Node structure for red-black tree
struct Node 
{
    int data;
    Node* left;
    Node* right;
    Node* parent;
    Color color;
    
    // Constructor for node
    explicit Node(int value) 
        : data(value), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
};

// Integer Set Class using Red-Black Tree implementation
class CSet
{
private:
    Node* root;     // Root of the red-black tree
    int size;       // Number of elements in the set
    
    // Helper functions for red-black tree operations
    void leftRotate(Node* x);
    void rightRotate(Node* y);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* x) const;
    void deleteNode(Node* z);
    Node* searchNode(int item) const;
    void destroyTree(Node* node);
    void copyTree(Node*& dest, const Node* src, Node* parent);
    void inOrderTraversal(Node* node, int* arr, int& index) const;
    
public:
    /**
     * Default constructor - creates an empty set
     */
    CSet();
    
    /**
     * Copy constructor - creates a deep copy of another set
     * @param other The set to copy from
     */
    CSet(const CSet& other);
    
    /**
     * Assignment operator - assigns contents from another set
     * @param other The set to assign from
     * @return Reference to this object
     */
    CSet& operator=(const CSet& other);
    
    /**
     * Equality operator - checks if two sets contain the same elements
     * @param other The set to compare with
     * @return true if sets are equal, false otherwise
     */
    bool operator==(const CSet& other) const;
    
    /**
     * Inequality operator - checks if two sets are different
     * @param other The set to compare with
     * @return true if sets are different, false otherwise
     */
    bool operator!=(const CSet& other) const;
    
    /**
     * Destructor - releases all resources
     */
    ~CSet();
    
    /**
     * Checks if an item exists in the set
     * @param item The item to check
     * @return true if item exists, false otherwise
     */
    bool isExist(int item) const;
    
    /**
     * Checks if two sets are equal
     * @param other The set to compare with
     * @return true if sets are equal, false otherwise
     */
    bool isEqual(const CSet& other) const;
    
    /**
     * Creates a new set containing the intersection with another set
     * @param other The set to intersect with
     * @return A new set containing common elements
     */
    CSet intersection(const CSet& other) const;
    
    /**
     * Creates a new set containing the union with another set
     * @param other The set to unite with
     * @return A new set containing all elements from both sets
     */
    CSet getUnion(const CSet& other) const;
    
    /**
     * Removes an item from the set
     * @param item The item to remove
     * @return true if item was found and removed, false otherwise
     */
    bool removeItem(int item);
    
    /**
     * Adds an item to the set if it doesn't already exist
     * @param item The item to add
     * @return true if item was added successfully, false otherwise
     */
    bool addItem(int item);
    
    /**
     * Gets an item at the specified position in sorted order
     * @param position The position (0-based index)
     * @return The item at the position or throws exception if invalid position
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
     * Displays the set elements in sorted order
     */
    void display() const;
};

// Default constructor
CSet::CSet() : root(nullptr), size(0) {}

// Copy constructor
CSet::CSet(const CSet& other) : root(nullptr), size(0)
{
    if (other.root != nullptr)
    {
        try
        {
            copyTree(root, other.root, nullptr);
            size = other.size;
        }
        catch (...)
        {
            destroyTree(root);
            root = nullptr;
            size = 0;
            throw;
        }
    }
}

// Assignment operator
CSet& CSet::operator=(const CSet& other)
{
    if (this != &other)
    {
        CSet temp(other);
        
        std::swap(root, temp.root);
        std::swap(size, temp.size);
    }
    return *this;
}

// Equality operator
bool CSet::operator==(const CSet& other) const
{
    return isEqual(other);
}

// Inequality operator
bool CSet::operator!=(const CSet& other) const
{
    return !isEqual(other);
}

// Destructor
CSet::~CSet()
{
    destroyTree(root);
}

// Clear all elements
void CSet::clear()
{
    destroyTree(root);
    root = nullptr;
    size = 0;
}

// Helper function to copy tree
void CSet::copyTree(Node*& dest, const Node* src, Node* parent)
{
    if (src == nullptr) return;
    
    dest = new Node(src->data);
    dest->color = src->color;
    dest->parent = parent;
    
    copyTree(dest->left, src->left, dest);
    copyTree(dest->right, src->right, dest);
}

// Helper function to destroy tree
void CSet::destroyTree(Node* node)
{
    if (node == nullptr) return;
    
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// Left rotation operation for balancing the tree
void CSet::leftRotate(Node* x)
{
    if (x == nullptr || x->right == nullptr)
        return;
        
    Node* y = x->right;
    x->right = y->left;
    
    if (y->left != nullptr)
    {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr)
    {
        root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

// Right rotation operation for balancing the tree
void CSet::rightRotate(Node* y)
{
    if (y == nullptr || y->left == nullptr)
        return;
        
    Node* x = y->left;
    y->left = x->right;
    
    if (x->right != nullptr)
    {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == nullptr)
    {
        root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

// Fix tree properties after insertion
void CSet::insertFixup(Node* z)
{
    while (z != root && z->parent != nullptr && z->parent->color == RED)
    {
        if (z->parent->parent == nullptr)
            break; // Protect against null parent's parent
            
        if (z->parent == z->parent->parent->left)
        {
            Node* y = z->parent->parent->right;
            if (y != nullptr && y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(z);
                }
                if (z->parent != nullptr && z->parent->parent != nullptr) {
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
        }
        else
        {
            Node* y = z->parent->parent->left;
            if (y != nullptr && y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(z);
                }
                if (z->parent != nullptr && z->parent->parent != nullptr) {
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
    }
    root->color = BLACK;
}

// Find the minimum value node in a subtree
Node* CSet::minimum(Node* x) const
{
    if (x == nullptr)
        return nullptr;
        
    while (x->left != nullptr)
    {
        x = x->left;
    }
    return x;
}

// Replace subtree u with subtree v
void CSet::transplant(Node* u, Node* v)
{
    if (u == nullptr)
        return;
        
    if (u->parent == nullptr)
    {
        root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    
    if (v != nullptr)
    {
        v->parent = u->parent;
    }
}

// Fix tree properties after deletion
void CSet::deleteFixup(Node* x)
{
    Node* w;
    while (x != root && (x == nullptr || x->color == BLACK))
    {
        if (x == nullptr)
            break;
            
        if (x->parent == nullptr)
            break;
            
        if (x == x->parent->left)
        {
            w = x->parent->right;
            if (w == nullptr)
                break;
                
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            
            if (w == nullptr)
                break;
                
            if ((w->left == nullptr || w->left->color == BLACK) && 
                (w->right == nullptr || w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right == nullptr || w->right->color == BLACK)
                {
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                
                if (w != nullptr) {
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    leftRotate(x->parent);
                }
                x = root;
            }
        }
        else
        {
            w = x->parent->left;
            if (w == nullptr)
                break;
                
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            
            if (w == nullptr)
                break;
                
            if ((w->right == nullptr || w->right->color == BLACK) && 
                (w->left == nullptr || w->left->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left == nullptr || w->left->color == BLACK)
                {
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                
                if (w != nullptr) {
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    rightRotate(x->parent);
                }
                x = root;
            }
        }
    }
    
    if (x != nullptr)
        x->color = BLACK;
}

// Delete a node from the tree
void CSet::deleteNode(Node* z)
{
    if (z == nullptr)
        return;
        
    Node* y = z;
    Node* x = nullptr;
    Color yOriginalColor = y->color;
    
    if (z->left == nullptr)
    {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nullptr)
    {
        x = z->left;
        transplant(z, z->left);
    }
    else
    {
        y = minimum(z->right);
        if (y == nullptr) {
            delete z;
            return;
        }
        
        yOriginalColor = y->color;
        x = y->right;
        
        if (y->parent == z)
        {
            if (x != nullptr)
                x->parent = y;
        }
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            if (y->right != nullptr)
                y->right->parent = y;
        }
        
        transplant(z, y);
        y->left = z->left;
        if (y->left != nullptr)
            y->left->parent = y;
        y->color = z->color;
    }
    
    delete z;
    
    if (yOriginalColor == BLACK && x != nullptr)
    {
        deleteFixup(x);
    }
}

// Search for a node with given value
Node* CSet::searchNode(int item) const
{
    Node* current = root;
    while (current != nullptr)
    {
        if (item == current->data)
            return current;
        else if (item < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// Check if an item exists in the set
bool CSet::isExist(int item) const
{
    return searchNode(item) != nullptr;
}

// Add an item to the set
bool CSet::addItem(int item)
{
    // Check if the item already exists
    if (isExist(item))
        return false;
    
    try
    {
        Node* z = new Node(item);
        Node* y = nullptr;
        Node* x = root;
        
        // Find the position to insert
        while (x != nullptr)
        {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }
        
        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->data < y->data)
            y->left = z;
        else
            y->right = z;
        
        // Fix red-black properties
        insertFixup(z);
        size++;
        return true;
    }
    catch (std::bad_alloc&)
    {
        // Handle memory allocation failure
        return false;
    }
}

// Remove an item from the set
bool CSet::removeItem(int item)
{
    Node* nodeToDelete = searchNode(item);
    if (nodeToDelete == nullptr)
        return false;
    
    deleteNode(nodeToDelete);
    size--;
    return true;
}

// In-order traversal to collect elements into an array
void CSet::inOrderTraversal(Node* node, int* arr, int& index) const
{
    if (node == nullptr) return;
    
    inOrderTraversal(node->left, arr, index);
    arr[index++] = node->data;
    inOrderTraversal(node->right, arr, index);
}

// Get item at specified position
int CSet::getItem(int position) const
{
    if (position < 0 || position >= size)
        throw std::out_of_range("Position out of range");
    
    try
    {
        int* elements = new int[size];
        int index = 0;
        inOrderTraversal(root, elements, index);
        
        int result = elements[position];
        delete[] elements;
        
        return result;
    }
    catch (std::bad_alloc&)
    {
        throw std::runtime_error("Memory allocation failed in getItem");
    }
}

// Check if two sets are equal
bool CSet::isEqual(const CSet& other) const
{
    if (size != other.size)
        return false;
    
    try
    {
        // Get all elements from this set
        int* thisElements = new int[size];
        int index = 0;
        inOrderTraversal(root, thisElements, index);
        
        // Check if each element exists in the other set
        bool result = true;
        for (int i = 0; i < size; i++)
        {
            if (!other.isExist(thisElements[i]))
            {
                result = false;
                break;
            }
        }
        
        delete[] thisElements;
        return result;
    }
    catch (std::bad_alloc&)
    {
        // In case of memory allocation failure
        // Fallback to slower method that doesn't require large arrays
        Node* current = root;

        // TODO: Implement a fallback comparison method

        return false;
    }
}

// Get intersection with another set
CSet CSet::intersection(const CSet& other) const
{
    CSet result;
    
    try
    {
        if (size > 0 && other.size > 0) {
            // Choose the smaller set to iterate over
            const CSet& smallerSet = (size <= other.size) ? *this : other;
            const CSet& largerSet = (size <= other.size) ? other : *this;
            
            int* elements = new int[smallerSet.size];
            int index = 0;
            smallerSet.inOrderTraversal(smallerSet.root, elements, index);
            
            for (int i = 0; i < smallerSet.size; i++)
            {
                if (largerSet.isExist(elements[i]))
                {
                    result.addItem(elements[i]);
                }
            }
            
            delete[] elements;
        }
        
        return result;
    }
    catch (std::bad_alloc&)
    {
        // In case of memory allocation failure
        return result; // Return empty set
    }
}

// Get union with another set
CSet CSet::getUnion(const CSet& other) const
{
    CSet result;
    
    try
    {
        // Add all elements from this set
        int* thisElements = new int[size];
        int index = 0;
        inOrderTraversal(root, thisElements, index);
        
        for (int i = 0; i < size; i++)
        {
            result.addItem(thisElements[i]);
        }
        
        // Add elements from the other set
        int* otherElements = new int[other.size];
        index = 0;
        other.inOrderTraversal(other.root, otherElements, index);
        
        for (int i = 0; i < other.size; i++)
        {
            result.addItem(otherElements[i]);
        }
        
        delete[] thisElements;
        delete[] otherElements;
        return result;
    }
    catch (std::bad_alloc&)
    {
        // In case of memory allocation failure
        // Try a direct node traversal approach without intermediate arrays

        // Copy all of this set's elements
        Node* current = root;
        // TODO: This would be implemented with a stack-based traversal instead
        // of recursion to avoid memory allocation issues
        
        // Then add all elements from other set
        // Also using direct traversal

        return result;
    }
}

// Get the size of the set
int CSet::getSize() const
{
    return size;
}

// Display the set elements
void CSet::display() const
{
    std::cout << "{ ";
    
    if (size > 0)
    {
        try
        {
            int* elements = new int[size];
            int index = 0;
            inOrderTraversal(root, elements, index);
            
            for (int i = 0; i < size; i++)
            {
                std::cout << elements[i];
                if (i < size - 1)
                {
                    std::cout << ", ";
                }
            }
            
            delete[] elements;
        }
        catch (std::bad_alloc&)
        {
            std::cout << "Memory error during display";
        }
    }
    
    std::cout << " }" << std::endl;
}

// Example main function to test the CSet class
int main()
{
    try
    {
        // Create and initialize sets
        CSet set1;
        set1.addItem(1);
        set1.addItem(3);
        set1.addItem(5);
        
        CSet set2;
        set2.addItem(3);
        set2.addItem(5);
        set2.addItem(7);
        
        // Display sets
        std::cout << "Set 1: ";
        set1.display();
        std::cout << "Set 2: ";
        set2.display();
        
        // Test set operations
        std::cout << "Intersection: ";
        CSet intersection = set1.intersection(set2);
        intersection.display();
        
        std::cout << "Union: ";
        CSet unionSet = set1.getUnion(set2);
        unionSet.display();
        
        // Test other operations
        std::cout << "Is 3 in Set 1? " << (set1.isExist(3) ? "Yes" : "No") << std::endl;
        std::cout << "Are sets equal? " << (set1.isEqual(set2) ? "Yes" : "No") << std::endl;
        
        set1.removeItem(3);
        std::cout << "Set 1 after removing 3: ";
        set1.display();
        
        // Test operator overloads
        CSet set3 = set1;  // Copy constructor
        std::cout << "Set 3 (copy of Set 1): ";
        set3.display();
        
        std::cout << "Set 1 == Set 2: " << (set1 == set2 ? "true" : "false") << std::endl;
        std::cout << "Set 1 != Set 2: " << (set1 != set2 ? "true" : "false") << std::endl;
        
        // Test exception handling for getItem
        try
        {
            std::cout << "Item at position 0 in Set 1: " << set1.getItem(0) << std::endl;
            std::cout << "Item at position 10 in Set 1: " << set1.getItem(10) << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}