# engine project (shared lib)
project(engine VERSION 0.0.1)

# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_library(${PROJECT_NAME} ${SRC_FILES})

# define symbols
add_definitions(-DFUSE_EXPORT)
add_definitions(-D_DEBUG)

# windows platform
if (WIN32)
  target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_image SDL2_ttf SDL2_mixer yaml-cppd)
  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/windows/includes)
  target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/windows/libs)
endif (WIN32)

# linux
if (UNIX)
    # search libs
    find_package(SDL2 REQUIRED)
    find_package(yaml-cpp REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_image REQUIRED)
    # include headers
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${YAML_CPP_INCLUDE_DIR})
    # link lib files
    target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})    
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})    
    target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})    
endif (UNIX)

# precompile headers
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# add include directory
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
