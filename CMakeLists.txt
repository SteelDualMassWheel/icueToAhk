#ustawienie wersji CMake
cmake_minimum_required(VERSION 3.7)

#nazwa projektu
project(lab_ppo)

# dodanie pliku wykonywalnego
# add_executable(Main *.cpp)

file(GLOB SOURCES "./*.cpp")
add_executable(main ${SOURCES})

#w przypadku kilku plików źródłowych
#add_executable(Main main.cpp test.cpp files.cpp)