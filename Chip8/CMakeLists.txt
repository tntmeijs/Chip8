project(Chip8)

# Include the headers located in "include/Chip8/"
include_directories(${PROJECT_SOURCE_DIR}/include/)

set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/Chip8/Utility/DataTypes.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Utility/Disassembler.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Emulator/Processor.hpp)

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/source/Main.cpp
    ${PROJECT_SOURCE_DIR}/source/Disassembler.cpp
    ${PROJECT_SOURCE_DIR}/source/Processor.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin)

add_executable(Chip8 ${SOURCE_FILES} ${HEADER_FILES})

# Copy the ROM files to the "/bin/" folder
file(COPY ${PROJECT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Also copy the ROM files to the root project folder, as this will allow users to run the application from within an IDE like Visual Studio itself
file(COPY ${PROJECT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR})