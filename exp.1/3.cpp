/*
3. Create a class CCircle, and finish the following demands:
    3.1 Define a member function, Distance, to get the distance between two points.
    3.2 Define a member function, Relationship, to judge the relationship between two circles, such as seperated, circumscribed, overlapped, inscribed, contained, coincide or other;
Note:
(1) You can define appropriate member variables and parameters according to your member functions.
(2) The CCircle you defined is used as following codes.
int main()
{
    CCircle c1(3); // 3 is radius
    CCircle c2(2, 4, 3); // 2 is radius, 4 is x-coordinate,3 is y-coordiante.
    cout << "The relationship is " << c1.Relationship(c2) << endl;
    return 0;
}
Output is:
    The relationship is circumscribe.
*/

#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

using namespace std;

class CCircle 
{
private:
    // Circle properties
    double radius;  // Radius of the circle
    double x;       // X-coordinate of center
    double y;       // Y-coordinate of center
    
    // Threshold value for floating-point comparisons
    static constexpr double EPSILON = 1e-10;

public:
    // Relationship types between two circles
    enum class Relation 
    {
        SEPARATED,    // No common points
        CIRCUMSCRIBE, // Externally tangent
        OVERLAPPED,   // Partially overlapping
        INSCRIBE,     // Internally tangent
        CONTAINED,    // One circle inside another
        COINCIDE,     // Same circle
        OTHER         // Unexpected case
    };

    CCircle(double radius, double x = 0.0, double y = 0.0); // Constructor with radius and center coordinates

    // Member functions
    double distance(const CCircle& other) const; // Calculate distance between centers of two circles
    Relation determineRelation(const CCircle& other) const; // Determine relationship between two circles
    string relationToString(Relation relation) const; // Convert relationship enum to string
    string relationship(const CCircle& other) const; // Public interface for relationship determination
    
    // Accessor methods
    double getRadius() const { return radius; } // Get radius
    double getX() const { return x; } // Get X-coordinate
    double getY() const { return y; } // Get Y-coordinate
};

// Constructor with radius and center coordinates
CCircle::CCircle(double radius, double x, double y) : radius(radius), x(x), y(y)
{
    if (radius <= 0) 
    {
        throw invalid_argument("Radius must be positive");
    }
}

// Calculate distance between centers of two circles
double CCircle::distance(const CCircle& other) const
{
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}

// Determine the relationship between two circles
CCircle::Relation CCircle::determineRelation(const CCircle& other) const
{
    double dist = distance(other);
    double sumRadii = radius + other.radius;
    double diffRadii = fabs(radius - other.radius);
    
    if (fabs(dist) < EPSILON && fabs(radius - other.radius) < EPSILON) 
    {
        return Relation::COINCIDE;
    }
    else if (dist > sumRadii) 
    {
        return Relation::SEPARATED;
    }
    else if (fabs(dist - sumRadii) < EPSILON) 
    {
        return Relation::CIRCUMSCRIBE;
    }
    else if (dist < sumRadii && dist > diffRadii) 
    {
        return Relation::OVERLAPPED;
    }
    else if (fabs(dist - diffRadii) < EPSILON) 
    {
        return Relation::INSCRIBE;
    }
    else if (dist < diffRadii) 
    {
        return Relation::CONTAINED;
    }
    else 
    {
        return Relation::OTHER;
    }
}

// Convert relationship enum to string representation
string CCircle::relationToString(Relation relation) const
{
    switch (relation) 
    {
        case Relation::SEPARATED:    return "separated";
        case Relation::CIRCUMSCRIBE: return "circumscribe";
        case Relation::OVERLAPPED:   return "overlapped";
        case Relation::INSCRIBE:     return "inscribe";
        case Relation::CONTAINED:    return "contained";
        case Relation::COINCIDE:     return "coincide";
        case Relation::OTHER:        return "other";
        default:                     return "unknown";
    }
}

// Public interface for relationship determination
string CCircle::relationship(const CCircle& other) const
{
    Relation relation = determineRelation(other);
    return relationToString(relation);
}

// Test the CCircle class and its relationship determination
int main()
{
    CCircle c1(3); // 3 is radius
    CCircle c2(2, 4, 3); // 2 is radius, 4 is x-coordinate,3 is y-coordiante.
    cout << "The relationship is " << c1.relationship(c2) << endl;
    return 0;
}