cmake_minimum_required(VERSION 3.16)
project(BookstoreSystem)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(code src/main.cpp src/UserManager.cpp src/UserManager.h src/HashTable.cpp src/HashTable.h src/DataOperation.cpp src/DataOperation.h)