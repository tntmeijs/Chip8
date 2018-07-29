project(Chip8)

set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/Chip8/Utility/DataTypes.hpp)

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/source/Main.cpp)

# Include the headers located in "include/Chip8/"

add_executable(Chip8 ${SOURCE_FILES} ${HEADER_FILES})