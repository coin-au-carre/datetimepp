#include "datetime.h"

#include <iostream>

int main() 
{
    using namespace datetime;

    auto t1 = Time(std::chrono::hours(2));
    std::cout << "Time simple = " << t1 << std::endl;

    auto t2 = Time(std::chrono::hours(2), date::days(1), std::chrono::seconds(78));
    std::cout << "Time variadic = " << t2 << std::endl;
}


