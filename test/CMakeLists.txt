cmake_minimum_required(VERSION 2.8)

project(datetime-test)


set (SRC_FILES 
    run_test.cpp
    date_test.cpp
)

set(HEADER_DIRS ${PROJECT_SOURCE_DIR}/..)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/.)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${name} PROPERTY CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
else()
    link_directories(${CMAKE_BINARY_DIR})
    target_link_libraries(${PROJECT_NAME} curl)
endif()

# colorization and auto registration of test cases
add_definitions(-Dlest_FEATURE_COLOURISE=1 -Dlest_FEATURE_AUTO_REGISTER=1) 

add_test(NAME datetimepp-test COMMAND ${PROJECT_NAME})

