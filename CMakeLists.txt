cmake_minimum_required(VERSION 3.27.0)
project(ChaosInterview VERSION 1.0.0 LANGUAGES C CXX)

# add_library(ChaosInterview ChaosInterview.cpp)

add_subdirectory(src)
add_subdirectory(app)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
