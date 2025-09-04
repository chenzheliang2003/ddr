set(TARGET_NAME ddr)

file(GLOB_RECURSE DDR_SOURCES src/*.cpp)
file(GLOB_RECURSE DDR_HEADERS include/*.hpp)

add_library(${TARGET_NAME} SHARED ${DDR_SOURCES} ${DDR_HEADERS} ${RESOURCES})

target_compile_definitions(ddr PRIVATE figure_EXPORTS)

if(WIN32)
    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME ${TARGET_NAME}
        PREFIX ""
        SUFFIX ".dll"
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden)
else()
    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME ${TARGET_NAME}
        PREFIX ""
        SUFFIX ".so"
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden)
endif()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${TARGET_NAME} PUBLIC
    modules
    bex
    ${ARMADILLO_LIBRARIES})