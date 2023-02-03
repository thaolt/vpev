cmake_minimum_required(VERSION 3.5)

project(vpev LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


INCLUDE_DIRECTORIES(/home/thaolt/Workspaces/gamble/PokerHandEvaluator/cpp/include)

add_executable(vpev main.cpp)

target_link_libraries(vpev
    /home/thaolt/Workspaces/gamble/PokerHandEvaluator/cpp/build/libpheval.a
)

install(TARGETS vpev
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
