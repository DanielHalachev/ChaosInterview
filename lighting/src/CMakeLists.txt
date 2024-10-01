file(GLOB PRIVATE_HEADER_LIST CONFIGURE_DEPENDS "${ChaosInterview_SOURCE_DIR}/src/*.h")
file(GLOB PUBLIC_HEADER_LIST CONFIGURE_DEPENDS "${ChaosInterview_SOURCE_DIR}/include/lighting/*.h")

# Create an INTERFACE library for a header-only implementation
add_library(lighting INTERFACE)

# Specify include directories
target_include_directories(lighting INTERFACE
    $<BUILD_INTERFACE:${ChaosInterview_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
