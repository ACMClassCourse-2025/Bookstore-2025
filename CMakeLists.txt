cmake_minimum_required(VERSION 3.28)
project(Bookstore_2025)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Bookstore_2025
        include/Memory.h
        src/main.cpp
        include/User.h
        include/Account.h
        include/DataMap.h
        src/Account.cpp
        include/Book.h
        include/Repository.h
        src/Repository.cpp
        src/Account.cpp
        前置作业/最终版本.cpp)
