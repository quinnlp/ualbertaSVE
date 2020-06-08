cmake_minimum_required(VERSION 3.8)
project(SVELoopPass)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add LLVM to build.
find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include_directories(${LLVM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})

# Export compile commands db.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect SVELoopPass src.
set(srcs
  "${CMAKE_CURRENT_SOURCE_DIR}/SVELoopPass.cpp"
)

# Create the SVELoopPass library.
llvm_add_library(SVELoopPass MODULE ${srcs})
