cmake_minimum_required(VERSION 3.15)

# 加载工具链文件
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-toolchain.cmake")
message(STATUS "Loading toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# 声明项目
project(template)

# 启用生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 启用 Ninja 颜色支持
add_compile_options(-fdiagnostics-color=always)

# 定义 SDK 路径
set(MSPM0_SDK_DIR "E:/software/TI/m0_sdk/mspm0_sdk_2_02_00_05" CACHE PATH "MSPM0 SDK 路径")

# 定义syscfg相关路径变量
set(PROJECT_SYSCFG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/syscfg")
set(PROJECT_SYSCFG_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/syscfg/${PROJECT_NAME}.syscfg")

# # 读取 device.opt 文件内容
# file(READ "syscfg/device.opt" DEVICE_OPT_CONTENTS)
# string(STRIP "${DEVICE_OPT_CONTENTS}" DEVICE_OPT_CONTENTS)

# # 添加为编译选项
# target_compile_options(${PROJECT_NAME} PRIVATE
#     ${DEVICE_OPT_CONTENTS}
# )

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

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEVICE_OPT}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEVICE_OPT}")

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
    "${MSPM0_SDK_DIR}/source/ti/driverlib/"
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
    "-Wl,-t${PROJECT_SOURCE_DIR}/syscfg/device_linker.cmd"
    # "-Wl,-t${PROJECT_SOURCE_DIR}/syscfg/device_linker.lds"
    "-Wl,-m,${PROJECT_NAME}.map"
    "-Wl,--entry_point=_c_int00"
    "-Wl,--rom_model"
    "-Wl,--warn_sections"
    "-L${MSPM0_SDK_DIR}/source"
    "-L${PROJECT_SOURCE_DIR}"
    "-L$ENV{TICLANG_ARMCOMPILER}/lib"  # 指定库文件路径
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    # sysbm
    # c
    ${ARM_COMPILER_ROOT}/lib/libsysbm.a
    ${ARM_COMPILER_ROOT}/lib/libc.a
    ${MSPM0_SDK_DIR}/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a
)


# 输出目录配置
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# 添加自定义命令生成 .bin 文件
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.bin
    COMMENT "Generating file"
)

# 清理规则
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
    "${CMAKE_BINARY_DIR}"
)