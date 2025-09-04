add_subdirectory(core)
set(TARGET_NAME modules)
add_library(${TARGET_NAME} INTERFACE)

target_link_libraries(${TARGET_NAME} INTERFACE
    core
)
