cmake_minimum_required(VERSION 3.10)
project(ddr VERSION 1.0.0 LANGUAGES CXX)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖包
find_package(Eigen3 REQUIRED)

# 设置第三方库路径
set(BEX_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

# 导入bex库
add_library(bex SHARED IMPORTED)
set_target_properties(bex PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${BEX_SDK_DIR}/include
    IMPORTED_LOCATION ${BEX_SDK_DIR}/lib/libbex.dylib
    IMPORTED_IMPLIB ${BEX_SDK_DIR}/lib/libbex.dylib
)

# 收集源文件
file(GLOB_RECURSE SOURCES 
    "include/*.hpp" 
    "include/aarand/*.hpp" 
    "include/subpar/*.hpp" 
    "include/knncolle/*.hpp" 
    "include/irlba/*.hpp" 
    "include/umappp/*.hpp" 
    "src/*.cpp"
)

# 创建主库
add_library(ddr SHARED ${SOURCES})

# 设置库属性
set_target_properties(ddr PROPERTIES
    OUTPUT_NAME "main"
    PREFIX ""
    SUFFIX ".dylib"
    # C_VISIBILITY_PRESET hidden
    # CXX_VISIBILITY_PRESET hidden
)

# 设置包含目录
target_include_directories(ddr PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/aarand>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/subpar>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/knncolle>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/irlba>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/umappp>
    ${EIGEN3_INCLUDE_DIRS}
    /opt/homebrew/opt/openblas/include
)

# 设置链接库
target_link_libraries(ddr PUBLIC
    /opt/homebrew/opt/openblas/lib/libopenblas.dylib
    bex
    Eigen3::Eigen
)

# 处理示例程序
file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
foreach(example_source ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_source} NAME_WE)
    
    add_executable(${example_name} ${example_source})
    target_link_libraries(${example_name} PRIVATE ddr)
    
    set_target_properties(${example_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples
    )
endforeach()

# 安装规则
install(TARGETS ddr
    LIBRARY DESTINATION /Applications/Baltamatica.app/Contents/plugins/ddr
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

foreach(example_source ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_source} NAME_WE)
    install(TARGETS ${example_name}
        RUNTIME DESTINATION bin/examples
    )
endforeach()