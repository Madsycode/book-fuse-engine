# runtime project (.exe)
project(game VERSION 0.0.1)

# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# link libraries
target_link_libraries(${PROJECT_NAME} engine)

# includes directories
include_directories(${CMAKE_SOURCE_DIR}/engine/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)