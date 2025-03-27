# 德州仪器MSPM0系列单片机的CMake工程模板

用develop分支的，master分支是我自己根据sdk中的makefile改的，基本不可用  
修改自改工程：https://github.com/zhzhongshi/MSPM0-CMAKE-GCC-Template.git

## 常见问题
编译没问题，但是烧录进去，无法运行


## 以下是原README.md  
# 德州仪器MSPM0系列单片机的CMake工程模板

支持Windows和Linux系统

可以支持多种交叉编译工具链，但是当前只为tiarmclang做了适配

依赖当前系统中的交叉编译工具链和MSPM0系列单片机的SDK。

**工程结构：**

```bash
project
├─.vscode		# vscode设置，配置了任务和快捷按钮
├─cmake 		# 设置交叉编译工具链的路径
├─lib			# 预留的第三方库
├─Source		# 主要源文件
│  ├─Inc
│  └─Src
└─syscfg		# 存放sysconfig工具生成的文件
```

## 一、使用方式

### 1、设置交叉编译工具

在`./cmake/arm-toolchain.cmake`中进行设置

设置交叉编译工具的安装路径，如：

使用tiarmclang

```cmake
# 编译器路径设置
set(TI_ARM_COMPILER_ROOT "E:/software/TI/ti_cgt_armllvm")
set(CMAKE_C_COMPILER "${TI_ARM_COMPILER_ROOT}/bin/tiarmclang.exe")
set(CMAKE_CXX_COMPILER "${TI_ARM_COMPILER_ROOT}/bin/tiarmclang.exe")
set(CMAKE_ASM_COMPILER "${TI_ARM_COMPILER_ROOT}/bin/tiarmclang.exe")
```

（如果是Linux的话，删除`.exe`）

在顶层cmakelists中会有如下语句设置交叉编译器：

```cmake
# 加载工具链文件
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-toolchain.cmake")
message(STATUS "Loading toolchain: ${CMAKE_TOOLCHAIN_FILE}")
```

### 2、设置SDK路径

在`./CMakeLists.txt`中进行设置

```cmake
# 定义 SDK 路径
set(MSPM0_SDK_DIR "E:/software/TI/m0_sdk/mspm0_sdk_2_02_00_05" CACHE PATH "MSPM0 SDK 路径")
```

### 3、设置启动文件
在`./CMakeLists.txt`中进行设置

这里设置的是`mspm0g350x`系列单片机，如果是其他系列，启动文件也应该在这里，需要改文件名。以具体文件路径为准

```cmake
list(APPEND SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/syscfg/ti_msp_dl_config.c"           # 添加 syscfg 生成的文件
    "${MSPM0_SDK_DIR}/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c"  # 添加启动文件
)
```


### 4、编译流程

本工程默认使用CMake+ninja的方法，也可以使用cmake+make

流程中的各个指令可以参考`.vscode/tasks.json`中的内容

具体如下：

```bash
# 生成Ninja构建系统，并将构建目录设置为build
cmake -G Ninja -B build

# 在build目录下构建所有目标
cmake --build build --target all

# 清理工程，删除build目录
cmd /c "rmdir /s /q build && rmdir /s /q .cache" # windows的powershell
rm -rf build					 # linux的bash
```

## 二、常见问题

待补充

