#include "datetime.h"

#include <iostream>

int main() 
{
    using namespace datetime;

    auto d1 = Date::fromtimestamp(1497014839);
    std::cout << "Date::fromtimestamp(1497014839) = " << d1 << std::endl;

    auto now = std::chrono::system_clock::now();
    auto d2 = Date::fromtimestamp(
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() // must cast to seconds
    );
    std::cout << "fromtimestamp(time_since_epoch) = " << d2 << std::endl;
    std::cout << "substract these 2 dates : " << d2 - d1 << std::endl;

    // Operator +
    auto delta = TimeDelta(date::weeks(2));
    std::cout << "Add 2 weeks: " << d1 + delta << std::endl;

    std::cout << d1.strftime("%A %d. %B %Y") << std::endl;

    std::cout << "weekday() = " << d1.weekday() << std::endl;
    std::cout << "isoweekday() = " << d1.isoweekday() << std::endl;
}




