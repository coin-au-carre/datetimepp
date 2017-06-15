#include "datetime.h"

#include <iostream>

int main() 
{
    using namespace datetime;
    using namespace std::chrono;
    using std::cout; using std::endl;

    auto x1 = Date::today();
    cout << "Today's date is " << x1 << endl;

    auto x2 = DateTime<>::now(); // same result as DateTime<>::today()
    cout << "Today's datetime is " << x2 << endl;

    auto time_delta = TimeDelta(date::weeks(1), date::days(2), hours(3), seconds(240), microseconds(10000)); // arguments order does not matter
    cout << "Here the delta time we want to add " << time_delta << endl;

    cout << "Here the added result " << x2 + time_delta << endl;

    // The C++ version can allow seconds only precision and can directly take a timezone string
    auto x3 = DateTime<std::chrono::seconds>::now("Asia/Tehran");
    cout << "Time in " << x3.tzinfo() << " is " << x3 << endl;

    auto x4 = DateTime<>::strptime("21/11/92 16:30", "%d/%m/%y %H:%M");
    cout << "datetime from strptime is " << x4 << endl;

    auto x5 = DateTime<>::utcfromtimestamp(1497252490.0282006);
    std::cout << "UTC datetime from timestamp is " << x5 << ". Check timestamp back: " << x5.timestamp() << std::endl;

    std::cout << "You can substract two datetimes into a timedelta " << x5 - x4 << std::endl;
}
