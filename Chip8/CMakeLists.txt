project(Chip8)

# Include the headers located in "include/Chip8/"
include_directories(${PROJECT_SOURCE_DIR}/include/)

set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/Chip8/Utility/DataTypes.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Emulator/Processor.hpp)

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/source/Main.cpp
    ${PROJECT_SOURCE_DIR}/source/Processor.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin)

add_executable(Chip8 ${SOURCE_FILES} ${HEADER_FILES})