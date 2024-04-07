cmake_minimum_required(VERSION 3.1.0)
project (main)

# prevent for in-source building
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#add_definitions(-DERROR)

set(CMAKE_BUILD_TYPE Debug) #Release
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

### wersja z katalogiem bibliotek w include i plikow cpp w src
set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# add_library(...)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
#target_link_libraries(...)

add_custom_target(run ./main)
