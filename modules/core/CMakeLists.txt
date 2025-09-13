set(TARGET_NAME core)

file(GLOB_RECURSE CORE_SOURCES src/*.cpp)
file(GLOB_RECURSE CORE_HEADERS include/*.hpp)

add_library(${TARGET_NAME} STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${TARGET_NAME} PUBLIC ${Gettext_INCLUDE_DIR} Eigen3::Eigen)
