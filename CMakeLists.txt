cmake_minimum_required(VERSION 3.10)

project(DemonEngine VERSION 1.0)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR} *.cpp)

add_executable(DemonEngine ${SOURCES} GLAD/src/glad.c)

set(OpenGL_GL_PREFERENCE LEGACY)

# Find all the packages we need
find_package(SDL2 REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)
find_package(fmt REQUIRED)

# Set their include directories or something
target_include_directories(DemonEngine PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(DemonEngine PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(DemonEngine PUBLIC ${CMAKE_SOURCE_DIR} GLAD/include)
target_include_directories(DemonEngine PUBLIC ${CMAKE_SOURCE_DIR} PhysX)
target_include_directories(DemonEngine PUBLIC ${CMAKE_SOURCE_DIR})


target_link_directories(DemonEngine PUBLIC "Libs")


# link the libraries
target_link_libraries(DemonEngine ${SDL2_LIBRARIES})
target_link_libraries(DemonEngine ${OPENGL_LIBRARIES})
target_link_libraries(DemonEngine ${CMAKE_DL_LIBS})
target_link_libraries(DemonEngine ${ASSIMP_LIBRARIES})
target_link_libraries(DemonEngine fmt)
target_link_libraries(DemonEngine PhysX_static_64 PhysXCharacterKinematic_static_64 PhysXPvdSDK_static_64 PhysXExtensions_static_64 PhysXCooking_static_64 PhysXFoundation_static_64 PhysXVehicle_static_64 PhysXCommon_static_64)


include_directories("/usr/include")
include_directories(${CMAKE_SOURCE_DIR})


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

add_compile_definitions(_DEBUG)