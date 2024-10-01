cmake_minimum_required(VERSION 3.30.0)
project(ChaosInterview VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# find_package(MPFR REQUIRED)
# include_directories(${MPFR_INCLUDE_DIRS})
# find_package(GMP REQUIRED)
# include_directories(${GMP_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)

# include(CTest)
# enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
