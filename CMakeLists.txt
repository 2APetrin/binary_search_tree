cmake_minimum_required(VERSION 3.8)

project(trees LANGUAGES CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -g -O0 -DDUMP_MODE -DDEBUG -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(./src)
add_subdirectory(./test)

target_include_directories(avl     PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(compare PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
