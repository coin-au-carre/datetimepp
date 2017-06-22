#include "datetime.h"

#include <iostream>

int main() 
{
    using namespace datetime;

    auto x1 = DateTime<>::now();
    std::cout << x1 << std::endl;
    std::cout << x1.isoformat() << std::endl;

    x1 = DateTime<>::utcnow();
    std::cout << x1 << std::endl;

    // What is the time in Berlin with seconds precision ?
    auto x2 = DateTime<std::chrono::seconds>::now("Europe/Berlin");
    std::cout << x2 << std::endl;
    std::cout << x2.strftime("%A, %d. %B %Y %I:%M%p") << std::endl;
    std::cout << "x2.utcoffset() = " << x2.utcoffset() << std::endl;

    auto x3 = DateTime<>::fromtimestamp(1497252490.0282006);
    std::cout << "fromtimestamp(1497252490.0282006) = " << x3 << std::endl;
    std::cout << "fromtimestamp(1497252490.0282006).tzinfo() = " << x3.tzinfo() << std::endl;
    std::cout << "fromtimestamp(1497252490.0282006).timestamp() = " << x3.timestamp() << std::endl;

    auto x4 = DateTime<>::utcfromtimestamp(1497252490.0282006);
    std::cout << "utcfromtimestamp(1497252490.0282006) = " << x4 << std::endl;

    auto x5 = DateTime<>::strptime("21/11/06 16:30", "%d/%m/%y %H:%M");
    std::cout << x5 << std::endl;

    std::cout << x3.date() << ": " << x5.year() << '-'
        << x5.month() << '-' << x5.day() << std::endl;

    auto delta = TimeDelta(std::chrono::hours(2));
    auto x6 = x5 + delta;
    std::cout << "x5 + TimeDelta(hours(2)) = " << x6 << std::endl;

    std::cout << "x6 - x5 = " << x6 - x5 << std::endl;
    std::cout << "x6 < x5 ? "<< std::boolalpha << (x6 < x5) << std::endl;
}


