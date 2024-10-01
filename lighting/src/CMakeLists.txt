# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ChaosInterview_SOURCE_DIR}/include/*.h")
# add_library(lighting Vector.cpp Light.cpp Scene.cpp IOParser.cpp Utils.cpp ${HEADER_LIST})
# target_include_directories(lighting PUBLIC ../include)

file(GLOB PRIVATE_HEADER_LIST CONFIGURE_DEPENDS "${ChaosInterview_SOURCE_DIR}/src/*.h")
file(GLOB PUBLIC_HEADER_LIST CONFIGURE_DEPENDS "${ChaosInterview_SOURCE_DIR}/include/lighting/*.h")
add_library(lighting Utils.cpp ${PUBLIC_HEADER_LIST})
target_include_directories(lighting PUBLIC ../include)
target_include_directories(lighting PRIVATE ../src)

# target_link_libraries(lighting ${Boost_LIBRARIES})
# target_link_libraries(lighting ${MPFR_LIBRARIES})
