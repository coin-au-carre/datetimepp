#include "lest.h"

#define CASE( name ) lest_CASE( specification(), name )

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}


int main( int argc, char * argv[] )
{
    auto num_failure = lest::run(specification() , argc, argv, std::cout);
    if (num_failure == 0) 
    {
        std::cout << lest::green("Given tests passed\n");
    } 
    else 
    {
        std::cout << lest::red("Given tests failed\n");
    }
}

