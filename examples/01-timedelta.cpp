#include "datetime.h"

#include <iostream>

int main() 
{
    using namespace datetime;

    auto x1 = TimeDelta(std::chrono::microseconds(4000), std::chrono::microseconds(1000000));
    std::cout << x1 << std::endl;

    // note: bug with date::months
    auto x2 = TimeDelta(std::chrono::microseconds(2400), std::chrono::seconds(3600 * 25), date::weeks(2));
    std::cout << x2 << std::endl;

    auto x3 = TimeDelta(std::chrono::seconds(3600 * 23), std::chrono::microseconds(500000));
    std::cout << "Before: " << x3 << " After 4x: " << 4 * x3 << std::endl;
    std::cout << "Before: " << x3 << " After 1.2x: " << x3 * 1.2 << std::endl;


    auto x4 = TimeDelta(std::chrono::hours(4));
    auto x5 = TimeDelta(std::chrono::hours(-1));
    std::cout << "x4 + x5 = " << x4 + x5 << std::endl;
    std::cout << "x5 - x4 = " << x5 - x4 << std::endl;

    std::cout << x5 << std::endl;
}


