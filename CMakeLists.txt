cmake_minimum_required(VERSION 2.6)
project(brewpipp)

add_executable(brewpipp main.cpp)

install(TARGETS brewpipp RUNTIME DESTINATION bin)
