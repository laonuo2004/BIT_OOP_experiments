/* 
2. Here is a callback function -- namely called function by function pointer.
    2.1 Define function, DrawTriangle, to show an isosceles triangle with stars.
    2.2 Define function, DrawSquare, to show a square with pluses.
    2.3 Callback can call functions you define as following:
int main()
{
    int row; // numbers of shape's row

    cin >> row;
    Draw(DrawTriangle, row);

    cout << endl << endl;
    Draw(DrawSquare, row);

    return 0;
}
The output is here:

    *
   ***
  *****
 *******
*********

+ + + + +
+       +
+       +
+       +
+ + + + +
*/

#include <iostream>
using namespace std;

// Interface for shape drawing
class ShapeDrawer 
{
public:
    virtual void operator()(int row) const = 0;  // Function object pattern
    virtual ~ShapeDrawer() {}
};

// Triangle drawer implementation
class TriangleDrawer : public ShapeDrawer 
{
public:
    void operator()(int row) const override 
    {
        for (int i = 0; i < row; i++) 
        {
            // Print spaces before stars
            for (int j = 0; j < row - i - 1; j++) 
            {
                cout << " ";
            }
            
            // Print stars in triangular pattern
            for (int j = 0; j < 2 * i + 1; j++) 
            {
                cout << "*";
            }
            cout << endl;
        }
    }
};

// Square drawer implementation
class SquareDrawer : public ShapeDrawer 
{
public:
    void operator()(int row) const override 
    {
        for (int i = 0; i < row; i++) 
        {
            if (i == 0 || i == row - 1) 
            {
                // First and last rows
                for (int j = 0; j < row; j++) 
                {
                    cout << "+";
                    if (j < row - 1) cout << " ";
                }
            } 
            else 
            {
                // Middle rows
                cout << "+";
                for (int j = 0; j < 2 * (row - 2) + 1; j++) 
                {
                    cout << " ";
                }
                cout << "+";
            }
            cout << endl;
        }
    }
};

// Additional shapes can be easily added
// class CircleDrawer : public ShapeDrawer { ... }
// class StarDrawer : public ShapeDrawer { ... }

// Function pointer type definition
typedef void (*DrawFunction)(int);

// Singleton instances of shape drawers
static TriangleDrawer triangleDrawer;
static SquareDrawer squareDrawer;

// Drawing functions that can be passed as function pointers
void DrawTriangle(int row) 
{
    triangleDrawer(row);
}

void DrawSquare(int row) 
{
    squareDrawer(row);
}

// Generic drawing function that accepts any drawing function pointer
void Draw(DrawFunction drawer, int row) 
{
    drawer(row);
}

int main()
{
    int row; // numbers of shape's row

    cin >> row;
    Draw(DrawTriangle, row);

    cout << endl << endl;
    Draw(DrawSquare, row);

    return 0;
}