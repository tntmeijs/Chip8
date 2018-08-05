project(Chip8)

# Include own headers, GLFW headers, and GL3W headers
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/gl3w-master/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/glfw-3.2.1/include)

set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/thirdparty/gl3w-master/include/GL/gl3w.h
    ${PROJECT_SOURCE_DIR}/thirdparty/gl3w-master/include/GL/glcorearb.h
    ${PROJECT_SOURCE_DIR}/thirdparty/gl3w-master/include/KHR/khrplatform.h

    ${PROJECT_SOURCE_DIR}/include/Chip8/Utility/DataTypes.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Utility/Disassembler.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Emulator/Processor.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Emulator/Renderer.hpp
    ${PROJECT_SOURCE_DIR}/include/Chip8/Emulator/Window.hpp)

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/thirdparty/gl3w-master/src/gl3w.c

    ${PROJECT_SOURCE_DIR}/source/Main.cpp
    ${PROJECT_SOURCE_DIR}/source/Disassembler.cpp
    ${PROJECT_SOURCE_DIR}/source/Processor.cpp
    ${PROJECT_SOURCE_DIR}/source/Renderer.cpp
    ${PROJECT_SOURCE_DIR}/source/Window.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin)

# Do not build the docs, tests, or examples that come with GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    
# Add GLFW
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glfw-3.2.1)

add_executable(Chip8 ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Chip8 glfw ${GLFW_LIBRARIES})

# Copy the ROM files to the "/bin/" folder
file(COPY ${PROJECT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Also copy the ROM files to the root project folder, as this will allow users to run the application from within an IDE like Visual Studio itself
file(COPY ${PROJECT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR})