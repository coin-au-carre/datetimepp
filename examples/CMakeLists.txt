cmake_minimum_required(VERSION 2.8)

project(examples)

find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})

set (SOURCES_EXAMPLES 
    00-showcase.cpp
    01-timedelta.cpp
    02-date.cpp
    03-datetime.cpp
)

set (TARGETS_EXAMPLES
    00-showcase
    01-timedelta
    02-date
    03-datetime
)

foreach( name ${TARGETS_EXAMPLES} )
    add_executable(${name} ${name}.cpp ../date/tz.cpp)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD_REQUIRED ON)
    if(NOT WIN32)
        target_link_libraries(${name} ${CURL_LIBRARIES})
    else()
        link_directories(${CMAKE_BINARY_DIR})
        target_link_libraries(${name} curl) # for appveyor CI
    endif()
endforeach()

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if ("${CMAKE_MAJOR_VERSION}${CMAKE_MINOR_VERSION}" LESS 31)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
