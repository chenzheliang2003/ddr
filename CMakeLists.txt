cmake_minimum_required(VERSION 3.15)
project(dip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(bex SHARED IMPORTED)

if(WIN32)
    set(BEX_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bex_sdk/windows)
    set_target_properties(bex PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${BEX_SDK_DIR}/include
        IMPORTED_LOCATION ${BEX_SDK_DIR}/lib/libbex.dll
        IMPORTED_IMPLIB ${BEX_SDK_DIR}/lib/libbex.dll.a
    )
else()
    set(BEX_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bex_sdk/linux)
    set_target_properties(bex PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${BEX_SDK_DIR}/include
        IMPORTED_LOCATION ${BEX_SDK_DIR}/lib/libbex.so
        IMPORTED_IMPLIB ${BEX_SDK_DIR}/lib/libbex.so
    )
endif()

find_package(Eigen3 REQUIRED)
find_package(Armadillo REQUIRED)

enable_testing()


add_subdirectory(modules)
add_subdirectory(baltamatica)

if(WIN32)
    set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/baltamatica/plugins")
    install(
        FILES ${CMAKE_BINARY_DIR}/bin/ddr.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/ddr/
        RENAME main.dll
    )
else()
    set(CMAKE_INSTALL_PREFIX "/opt/Baltamatica/plugins")
    install(
        FILES ${CMAKE_BINARY_DIR}/lib/ddr.so
        DESTINATION ${CMAKE_INSTALL_PREFIX}/ddr/
        RENAME main.so
    )
endif()
