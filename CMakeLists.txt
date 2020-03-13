cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(main)

set(CENGINE_LIB cengine)

# you should modify THIS
set(LIBS ${CENGINE_LIB})
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux
    link_directories(${PROJECT_SOURCE_DIR}/lib/linux)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows
    add_definitions(/w"4996" /MTd)
    link_directories(${PROJECT_SOURCE_DIR}/lib)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE CENGINE_LIB_FILES ${PROJECT_SOURCE_DIR}/src/cengine/*)
add_library(${CENGINE_LIB} SHARED ${CENGINE_LIB_FILES})
target_compile_definitions(${CENGINE_LIB} PRIVATE -DCENGINE_LIB_EXPORT)
target_link_libraries(${CENGINE_LIB} lua)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_executable(main ${SRC_FILES})
target_link_libraries(main ${LIBS})
