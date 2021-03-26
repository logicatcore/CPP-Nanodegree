cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# for globbing /proc/ using filesystem
find_package(Boost REQUIRED filesystem)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES} ${Boost_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

# Add the testing executable
# add_subdirectory(thirdparty/googletest)

# add_executable(testt test/utest_monitor.cpp src/linux_parser.cpp src/format.cpp)

# target_link_libraries(testt gtest_main ${Boost_LIBRARIES})
