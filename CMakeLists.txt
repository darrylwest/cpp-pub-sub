cmake_minimum_required(VERSION 3.30)
project(pubsub VERSION 1.0.0 LANGUAGES CXX)

# Instead of FetchContent, use:
find_package(spdlog REQUIRED)

# Require C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add library target
add_library(pubsub INTERFACE)
target_include_directories(pubsub 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Add executable target
add_executable(pubsub_example src/main.cpp)
target_link_libraries(pubsub_example 
    PRIVATE 
        pubsub
        spdlog::spdlog
)

# Add threading support
find_package(Threads REQUIRED)
target_link_libraries(pubsub_example PRIVATE Threads::Threads)

# Optional: Installation rules
include(GNUInstallDirs)
install(TARGETS pubsub
    EXPORT pubsub-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/pubsub
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT pubsub-targets
    FILE pubsub-targets.cmake
    NAMESPACE pubsub::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pubsub
)

