file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ChaosInterview_SOURCE_DIR}/include/*.h")
add_library(lighting Vector.cpp Light.cpp Scene.cpp IOParser.cpp ${HEADER_LIST})
target_include_directories(lighting PUBLIC ../include)
