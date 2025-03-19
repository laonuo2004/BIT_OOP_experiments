/*
4. In C09: Cpptime.h of chapter 9，There is an example, Time, which used C library.
    4.1 Define CDateTime to encapusulate funcitons: localtime and struct tm in C library;
    4.2 In the main, the class can be used in te following way:
    int main()
    {
        CDateTime dt = CDateTime::Now();

        // 以 am 或 pm 形式显示当前时间，例如下午：3:30:12 pm
        dt.ShowTime12();

        // 以 24 小时形式显示当前时间，例如下午：22:11:12 pm
        dt.ShowTime24();

        // 显示当前日期和星期，例如：2025 年 3 月 19 日，星期三
        dt.ShowDate();

        return 0;
    }
    4.3 Furthermore, display current time dynamically.
*/

#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <conio.h>

class KeyboardInput
{
public:
    /**
     * Check if a key has been pressed
     * @return true if a key is available in the keyboard buffer
     */
    static bool keyPressed();
    
    /**
     * Get the pressed key without waiting
     * @return The ASCII code of the pressed key
     */
    static int getKey();
};

class CDateTime
{
private:
    // Constants for time formatting
    static const int SECONDS_PER_HOUR = 3600;
    static const int SECONDS_PER_DAY = 86400;
    
    // Member variables
    time_t rawTime;      // Raw time value (seconds since epoch)
    struct tm timeInfo;  // Broken-down time structure
    
    /**
     * Updates the time info structure with the current raw time
     * @return true if successful, false otherwise
     */
    bool updateTimeInfo();
    
    /**
     * Gets day of week name in Chinese
     * @param dayOfWeek Integer representing day of week (0-6, Sunday is 0)
     * @return Day name in Chinese
     */
    std::string getDayOfWeekName(int dayOfWeek) const;

public:
    /**
     * Default constructor - initializes with current time
     */
    CDateTime();
    
    /**
     * Static method to create a CDateTime object with current time
     * @return CDateTime object with current time
     */
    static CDateTime Now();
    
    /**
     * Display time in 12-hour format (e.g., 3:30:12 pm)
     */
    void showTime12() const;
    
    /**
     * Display time in 24-hour format (e.g., 22:11:12)
     */
    void showTime24() const;
    
    /**
     * Display date and day of week (e.g., 2025年3月19日，星期三)
     */
    void showDate() const;
    
    /**
     * Format time in 12-hour format
     * @return Formatted time string
     */
    std::string formatTime12() const;
    
    /**
     * Format time in 24-hour format
     * @return Formatted time string
     */
    std::string formatTime24() const;
    
    /**
     * Format date and day of week
     * @return Formatted date string
     */
    std::string formatDate() const;
};

/**
 * TimeDisplayHelper class to handle dynamic time display functionality
 */
class TimeDisplayHelper
{
private:
    static bool stopRequested;  // Flag to signal stopping display

public:
    /**
     * Display both time formats dynamically and simultaneously
     * until user presses 'q' to exit
     * @param updateInterval Interval between updates in milliseconds
     * @return true if completed normally, false if interrupted
     */
    static bool showDynamicTimeBoth(int updateInterval = 1000);
    
    /**
     * Request to stop any ongoing dynamic time display
     */
    static void requestStop();
    
    /**
     * Reset stop flag (automatically done at the start of showDynamicTime)
     */
    static void resetStopFlag();
};

// Initialize static member
bool TimeDisplayHelper::stopRequested = false;

// Implementation of KeyboardInput
bool KeyboardInput::keyPressed()
{
    return _kbhit() != 0;
}

int KeyboardInput::getKey()
{
    return _getch();
}

// Default constructor
CDateTime::CDateTime()
{
    time(&rawTime);
    if (!updateTimeInfo())
    {
        throw std::runtime_error("Failed to initialize time information");
    }
}

// Update time info structure from raw time
bool CDateTime::updateTimeInfo()
{
    return (localtime_s(&timeInfo, &rawTime) == 0);
}

// Static method to create a CDateTime with current time
CDateTime CDateTime::Now()
{
    return CDateTime();
}

// Get day of week name
std::string CDateTime::getDayOfWeekName(int dayOfWeek) const
{
    const std::string dayNames[] = {
        "日", "一", "二", "三", "四", "五", "六"
    };
    
    if (dayOfWeek >= 0 && dayOfWeek < 7)
    {
        return dayNames[dayOfWeek];
    }
    return "未知";
}

// Format time in 12-hour format
std::string CDateTime::formatTime12() const
{
    int hour12 = timeInfo.tm_hour % 12;
    if (hour12 == 0)
    {
        hour12 = 12;  // 12 AM/PM instead of 0
    }
    
    std::ostringstream oss;
    oss << std::setfill('0') 
        << hour12 << ":"
        << std::setw(2) << timeInfo.tm_min << ":"
        << std::setw(2) << timeInfo.tm_sec 
        << (timeInfo.tm_hour < 12 ? " am" : " pm");
        
    return oss.str();
}

// Display time in 12-hour format
void CDateTime::showTime12() const
{
    std::cout << formatTime12() << std::endl;
}

// Format time in 24-hour format
std::string CDateTime::formatTime24() const
{
    std::ostringstream oss;
    oss << std::setfill('0') 
        << std::setw(2) << timeInfo.tm_hour << ":"
        << std::setw(2) << timeInfo.tm_min << ":"
        << std::setw(2) << timeInfo.tm_sec;
        
    return oss.str();
}

// Display time in 24-hour format
void CDateTime::showTime24() const
{
    std::cout << formatTime24() << std::endl;
}

// Format date and day of week
std::string CDateTime::formatDate() const
{
    std::ostringstream oss;
    oss << (1900 + timeInfo.tm_year) << " 年 "
        << (timeInfo.tm_mon + 1) << " 月 "  // Direct use of month number
        << timeInfo.tm_mday << " 日，星期"
        << getDayOfWeekName(timeInfo.tm_wday);
        
    return oss.str();
}

// Display date and day of week
void CDateTime::showDate() const
{
    std::cout << formatDate() << std::endl;
}

// Reset stop flag
void TimeDisplayHelper::resetStopFlag()
{
    stopRequested = false;
}

// Request to stop dynamic display
void TimeDisplayHelper::requestStop()
{
    stopRequested = true;
}

// Display both 12-hour and 24-hour formats simultaneously
bool TimeDisplayHelper::showDynamicTimeBoth(int updateInterval)
{
    // Input validation
    if (updateInterval <= 0)
    {
        std::cerr << "Error: Invalid update interval for dynamic time display" << std::endl;
        return false;
    }
    
    // Reset stop flag at the beginning
    resetStopFlag();
    
    std::cout << "Dynamic time display running... (Press 'q' to stop and continue)" << std::endl;
    
    while (!stopRequested)
    {
        // Check for keyboard input
        if (KeyboardInput::keyPressed())
        {
            int key = KeyboardInput::getKey();
            // Check if 'q' or 'Q' was pressed
            if (key == 'q' || key == 'Q')
            {
                std::cout << std::endl << "Dynamic display stopped by user." << std::endl;
                return false;  // Interrupted by user
            }
        }
        
        // Clear the line (extra spaces to ensure previous content is overwritten)
        std::cout << "\r                                                              \r";
        
        // Get current time
        CDateTime currentTime = CDateTime::Now();
        
        // Display both formats simultaneously
        std::cout << "Current time: " 
                  << "[12h] " << currentTime.formatTime12() 
                  << " | [24h] " << currentTime.formatTime24() << std::flush;
        
        // Sleep for specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
    
    // Move to next line after finishing
    std::cout << std::endl;
    return true;  // Completed normally
}

// Main function to demonstrate the functionality
int main()
{
    try
    {
        std::cout << "===== Static Time Display =====" << std::endl;
        
        // Get current date and time
        CDateTime dt = CDateTime::Now();
        
        // Display in different formats
        std::cout << "12-hour format: ";
        dt.showTime12();
        
        std::cout << "24-hour format: ";
        dt.showTime24();
        
        std::cout << "Current date: ";
        dt.showDate();
        
        // Display dynamic time
        std::cout << "\n===== Dynamic Time Display =====" << std::endl;
        TimeDisplayHelper::showDynamicTimeBoth(1000);
        
        std::cout << "\n===== Demonstration Complete =====" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}