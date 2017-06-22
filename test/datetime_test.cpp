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


CASE("fromtimestamp" "[datetime]") 
{
    auto x = DateTime<>::fromtimestamp(1497252490.0282006);
    EXPECT(to_string(x) == "2017-06-12 09:28:10.028200626 CEST");
    // EXPECT(DateTime<>) // TODO work on DateTime CTOR
}



}

