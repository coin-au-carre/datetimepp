#include "lest.h"
#define CASE( name ) lest_CASE( specification(), name )
extern lest::tests & specification();

#include "datetime.h"

namespace 
{


template<class T>
std::string to_string(const T& obj) 
{
    std::stringstream ss;
    ss << obj;
    return ss.str();
}

using namespace datetime;


CASE("basic" "[date]") 
{
    auto d = Date(date::year(2017)/6/21);
    EXPECT(d.day()      == date::day(21));
    EXPECT(d.month()    == date::month(6));
    EXPECT(d.year()     == date::year(2017));
}


CASE("fromtimestamp" "[date]") 
{
    auto d1 = Date::fromtimestamp(1497014839);
    EXPECT(to_string(d1) == "2017-06-09");

    auto d2 = Date::fromtimestamp(1498052260);
    EXPECT(d2 == Date(date::year(2017)/6/21));
}



CASE("operation" "[date]") 
{
    auto d1 = Date(date::year(2017)/6/10);
    auto delta = TimeDelta(date::weeks(1), date::days(2)); // + 9 days
    
    EXPECT(d1 + delta == Date(date::year(2017)/6/19));
}

}

