/*
4. Creat a class CDate, and finish the following functions as below:
    4.1 Define a member function, Span, to get the number of days of two dates, such as :
        CDate date1(2025, 2, 24), date2(2025, 3, 12);
        int spanDays = date1.Span(date2);
    4.2 You may define other members as needed and appropriate parameters according to your member functions.
*/

#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

class CDate 
{
private:
    int year;
    int month;
    int day;
    
    // Helper methods
    bool isLeapYear(int y) const; // Check if a year is a leap year
    int getDaysInMonth(int y, int m) const; // Get number of days in a month
    long long getTotalDays() const; // Calculate total days from year 1 to current date
    
public:
    // Constructors
    CDate(int y = 1970, int m = 1, int d = 1); // Set default date to 1970-01-01
    
    // Core functionality
    int Span(const CDate& other) const; // Calculate span in days between two dates
    
    // The following functions are not required but can be useful
    // Utility methods
    void display() const; // Display date in YYYY-MM-DD format
    bool isValid() const; // Check if the date is valid
    
    // Accessor methods
    // Getters
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
    
    // Setters
    void setDate(int y, int m, int d); // Set a new date with validation
};

CDate::CDate(int y, int m, int d) : year(y), month(m), day(d) 
{
    if (!isValid())
    {
        throw invalid_argument("Invalid date");
    }
}

// Check if the year is leap year
bool CDate::isLeapYear(int y) const 
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Get the number of days in the specified month
int CDate::getDaysInMonth(int y, int m) const 
{
    static const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeapYear(y))
        return 29;
    return daysInMonth[m];
}

// Calculate the total days from year 1970 to the current date
long long CDate::getTotalDays() const 
{
    long long totalDays = 0;
    
    // Days from previous years
    for (int y = 1970; y < year; ++y)
    {
        totalDays += isLeapYear(y) ? 366 : 365;
    }
    
    // Days from previous months this year
    for (int m = 1; m < month; ++m)
    {
        totalDays += getDaysInMonth(year, m);
    }
    
    // Days this month
    totalDays += day;
    
    return totalDays;
}

// Calculate the span (in days) between two dates
int CDate::Span(const CDate& other) const 
{
    long long days1 = getTotalDays();
    long long days2 = other.getTotalDays();
    return abs(static_cast<int>(days1 - days2));
}

// Display the date in a readable format
void CDate::display() const 
{
    cout << year << "-" << month << "-" << day;
}

// Check if the current date is valid
bool CDate::isValid() const 
{
    if (year < 1970) return false;
    if (month < 1 || month > 12) return false;
    
    int maxDay = getDaysInMonth(year, month);
    if (day < 1 || day > maxDay) return false;
    
    return true;
}

// Set a new date with validation
void CDate::setDate(int y, int m, int d) 
{
    year = y;
    month = m;
    day = d;
    
    if (!isValid())
    {
        throw invalid_argument("Invalid date");
    }
}

// Test function
int main() 
{
    try 
    {
        CDate date1(2025, 2, 24);
        CDate date2(2025, 3, 12);
        
        int spanDays = date1.Span(date2);
        
        cout << "First date: ";
        date1.display();
        cout << endl;
        
        cout << "Second date: ";
        date2.display();
        cout << endl;
        
        cout << "The span between these two dates is " << spanDays << " days." << endl;
    }
    catch (const exception& e) 
    {
        cerr << "Error: " << e.what() << endl;
    }
    
    return 0;
}