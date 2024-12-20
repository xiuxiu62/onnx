cmake_minimum_required(VERSION 3.12)

project(
  onnx
  VERSION 0.1.0
  LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ONNX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "ONNX include directory")
set(ONNX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "ONNX include directory")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(RELEASE)
endif()


file(GLOB_RECURSE BIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library(${PROJECT_NAME}
  ${LIB_SOURCES}
  ${HEADERS}
)

add_executable(${PROJECT_NAME}-dev 
  ${BIN_SOURCES} 
)

target_link_libraries(${PROJECT_NAME}-dev PRIVATE
  ${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/onnx>
    $<INSTALL_INTERFACE:include/onnx>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME}-dev
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_custom_target(run
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
  COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-dev
  DEPENDS ${PROJECT_NAME}-dev
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Building and running ${PROJECT_NAME}-dev"
)



# cmake_minimum_required(VERSION 3.12)

# project(
#   onnx
#   VERSION 0.1.0
#   LANGUAGES C CXX
# )

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # Set up the generated include directory structure
# set(ONNX_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
# file(MAKE_DIRECTORY "${ONNX_GENERATED_INCLUDE_DIR}/onnx")

# # Create symbolic links or copy files
# add_custom_command(
#     OUTPUT "${ONNX_GENERATED_INCLUDE_DIR}/onnx"
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#             "${CMAKE_CURRENT_SOURCE_DIR}/include"
#             "${ONNX_GENERATED_INCLUDE_DIR}/onnx"
#     DEPENDS ${HEADERS}
# )

# # Set the new include directory
# set(ONNX_INCLUDE_DIR "${ONNX_GENERATED_INCLUDE_DIR}" CACHE PATH "ONNX include directory")

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Debug)
# endif()

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#   add_compile_definitions(DEBUG)
# elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#   add_compile_definitions(RELEASE)
# endif()

# file(GLOB_RECURSE BIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
# file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
# file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# add_library(${PROJECT_NAME}
#   ${LIB_SOURCES}
#   ${HEADERS}
# )

# add_executable(${PROJECT_NAME}-dev 
#   ${BIN_SOURCES} 
# )

# target_link_libraries(${PROJECT_NAME}-dev PRIVATE
#   ${PROJECT_NAME}
# )

# target_include_directories(${PROJECT_NAME} 
#   PUBLIC
#     $<BUILD_INTERFACE:${ONNX_GENERATED_INCLUDE_DIR}>
#     $<INSTALL_INTERFACE:include>
#   PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
# )

# target_include_directories(${PROJECT_NAME}-dev
#   PRIVATE
#     ${ONNX_GENERATED_INCLUDE_DIR}
# )

# add_custom_target(run
#   COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
#   COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-dev
#   DEPENDS ${PROJECT_NAME}-dev
#   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMENT "Building and running ${PROJECT_NAME}-dev"
# )
