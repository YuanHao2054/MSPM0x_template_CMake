cmake_minimum_required(VERSION 3.15)

# 加载工具链文件
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-toolchain.cmake")
message(STATUS "Loading toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# 声明项目
project(template)

# 启用生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 定义 SDK 路径
set(MSPM0_SDK_DIR "E:/software/TI/m0_sdk/mspm0_sdk_2_02_00_05" CACHE PATH "MSPM0 SDK 路径")

# 定义syscfg相关路径变量
set(PROJECT_SYSCFG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/syscfg")
set(PROJECT_SYSCFG_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.syscfg")

add_compile_options(
    -O2
    -gdwarf-3
    -mcpu=cortex-m0plus
    -mthumb
    -mfloat-abi=soft
    -ffunction-sections
    -fdata-sections
    -nostdlib
)

add_compile_definitions(
    DeviceFamily_MSPM0G
)

add_compile_definitions(
    __MSPM0G3507__
    __TI_ARM__
    __TI_ARM_CLANG_VERSION=3001000
)

# 包含目录
include_directories(
    "${MSPM0_SDK_DIR}/source"
    "${MSPM0_SDK_DIR}/source/third_party/CMSIS/Core/Include"
    "${PROJECT_SOURCE_DIR}/Source/Inc"
    "${PROJECT_SOURCE_DIR}/syscfg"
)

# 动态获取源文件列表
file(GLOB SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Source/Src/*.c"
)

list(APPEND SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/syscfg/ti_msp_dl_config.c"           # 添加 syscfg 生成的文件
    "${MSPM0_SDK_DIR}/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c"  # 添加启动文件
)

# 可执行目标
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 链接配置
target_link_options(${PROJECT_NAME} PRIVATE
    # "-Wl,-t${PROJECT_SOURCE_DIR}/syscfg/device_linker.cmd"
    "-Wl,-t${PROJECT_SOURCE_DIR}/syscfg/device_linker.lds"
    "-Wl,-m,${PROJECT_NAME}.map"
    "-Wl,--entry=_c_int00"
    "-Wl,--rom_model"
    "-Wl,--warn_sections"
    "-L${MSPM0_SDK_DIR}/source"
    "-L${PROJECT_SOURCE_DIR}"
    "-L$ENV{TICLANG_ARMCOMPILER}/lib"  # 指定库文件路径
    "-llibsysbm.a"  # 添加 libsysbm.a
    "-llibc.a"      # 添加 libc.a
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    sysbm
    c
    # ${ARM_COMPILER_ROOT}/lib/libsysbm.a
    # ${ARM_COMPILER_ROOT}/lib/libc.a
)


# 输出目录配置
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# 清理规则
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
    "${CMAKE_BINARY_DIR}"
)