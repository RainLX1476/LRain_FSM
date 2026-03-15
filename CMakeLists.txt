cmake_minimum_required(VERSION 3.10)

project(LRain_FSM)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#包含文件
include_directories(include)

# 源文件
set(SRC
	src/LRain_FSM.c
	src/port.c
)

add_library(LRain_FSM ${SRC})

set(IS_ARM_NONE_EABI FALSE)
if(CMAKE_C_COMPILER MATCHES "arm-none-eabi" OR CMAKE_CXX_COMPILER MATCHES "arm-none-eabi")
	set(IS_ARM_NONE_EABI TRUE)
	message(STATUS "Detected arm-none-eabi toolchain → disabling tests")
endif()

if(NOT IS_ARM_NONE_EABI)
	enable_testing()
	add_subdirectory(tests)
endif()




# 源文件
set(SRC
	examples/main.c
)

add_executable(examples ${SRC})

target_link_libraries(examples LRain_FSM)

