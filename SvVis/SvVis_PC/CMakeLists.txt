# CMakeList.txt : CMake project for SvVis_PC, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# c++ 17(inline variables)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project ("SvVis_PC")

# Add source to this project's executable.
add_executable (SvVis_PC "SvVis_PC.cpp" "SvVis_PC.hpp")

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SvVis_PC PRIVATE Threads::Threads)

# TODO: Add tests and install targets if needed.
