# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(FFTW3 REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

#find_package(FFTW3 REQUIRED)
set(FFTW3_INCLUDE_DIR /usr/include)
set(FFTW3_LIBRARIES /usr/lib/x86_64-linux-gnu/libfftw3.so)

# Find SFML library
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#find_package(SFML 2.5 COMPONENTS system REQUIRED)


# Add the executable
add_executable(MyProject src/main.cpp )

# Include directories
target_include_directories(MyProject PUBLIC "${PROJECT_BINARY_DIR}" include)
# Link FFTW library
#target_link_libraries(MyProject ${FFTW3_LIBRARIES}) 
target_link_libraries(MyProject PRIVATE ${FFTW3_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Link libraries
#target_link_libraries(MyProject PRIVATE ${FFTW3_LIBRARIES} OpenGL::GL GLUT::GLUT GLU::GLU glfw)
target_link_libraries(MyProject PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)

