/*
2. Overload the operator[ ] as a read-only member function in vector.

    #include <iostream>
    using namespace std;
    class vector
    {
    public:
        vector(int s)
        {
            size = s;
            v = new int[s];
            for(int i = 0; i < size; i++) v[i] = i * 10;
        }
        ~vector() { if (!v) { delete[] v; v = nullptr; } }

        // Read only: operator [ ]
    private:
        int *v, size;
    };

    int main()
    {
        vector vec(5);

        // vec[2] = 12; // ERROR: No modifications allowed

        int x = vec[2];

        return 0;
    }
*/

#include <iostream>
using namespace std;
class vector
{
public:
    vector(int s)
    {
        size = s;
        v = new int[s];
        for(int i = 0; i < size; i++) v[i] = i * 10;
    }
    ~vector() { if (!v) { delete[] v; v = nullptr; } }

    // Read only: operator [ ]
    const int& operator[](int index) const;
private:
    int *v, size;
};

/*
Use `const` to indicate that the operator[] is a read-only member function:
The former `const`:
    - Makes the return value (reference) constant, preventing modification of the referenced value
    - This is why "vec[2] = 12" causes a compilation error
The latter `const`:
    - Declares this as a constant member function that won't modify the object's state
    - Allows this operator to be called on const vector objects
*/
const int& vector::operator[](int index) const
{
    return v[index];
}

int main()
{
    vector vec(5);

    // vec[2] = 12; // ERROR: No modifications allowed

    int x = vec[2];

    return 0;
}