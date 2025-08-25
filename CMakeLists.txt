cmake_minimum_required(VERSION 3.10)
project(ddr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
)

file(GLOB_RECURSE SOURCES "include/*.hpp" "src/*.cpp")

add_library(ddr SHARED ${SOURCES})

find_package(Eigen3 REQUIRED)
get_target_property(EIGEN_INCLUDE_DIRS Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(ddr PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${EIGEN_INCLUDE_DIRS}
    /opt/homebrew/opt/openblas/include
)

target_link_libraries(ddr PUBLIC
    /opt/homebrew/opt/openblas/lib/libopenblas.dylib)

file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
foreach(example_source ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_source} NAME_WE)
    
    add_executable(${example_name} ${example_source})
    
    target_link_libraries(${example_name} PRIVATE ddr)
    
    set_target_properties(${example_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples
    )
    
    install(TARGETS ${example_name}
        RUNTIME DESTINATION bin/examples
    )
endforeach()

install(TARGETS ddr
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)