# LRain_FSM
凌雨—-有限状态机框架
# 简介
LRain_FSM是一个面向嵌入式应用的的有限状态机框架，致力于帮助开发者快速实现有限状态机。
# 特性
- 简单易用：使用C语言编写，只需要定义状态ID(使用枚举变量赋予名称)和状态句柄函数，即可快速实现有限状态机功能。
- 灵活扩展：用户可以创建并运行多个状态机实例，方便实现状态机嵌套等各种复杂结构
- 跨平台：不含任何平台扩展依赖可以在各种嵌入式系统及操作系统上运行。
- 时间戳支持：状态机内部维护当前状态运行时间，方便实现定时状态切换等功能。（需要用户实现时间戳获取函数）

# 快速开始

## 定义状态机
使用枚举变量定义状态ID，并为每个状态定义状态句柄函数。
```c
typedef enum {
	IDLE,
	RUNNING,
	ERROR
} state_t;

uint16_t idle_handler(fsm_t* fsm, fsm_event event) {
	// 处理IDLE状态
	if (event == FSM_EVENT_INIT) {
		// 初始化IDLE状态

	} else if (event == FSM_EVENT_DURING) {
		// IDLE状态持续执行

	} else if (event == FSM_EVENT_EXIT) {
		// 退出IDLE状态

	}
}

void running_handler(fsm_t* fsm, fsm_event event) {
    // 处理RUNNING状态
}

void error_handler(fsm_t* fsm, fsm_event event) {
    // 处理ERROR状态
}
```

## 创建状态机实例
创建状态机实例，将状态及其句柄函数装入状态机
```c
#include "LRain_FSM.h"

char buf[1024];

fsm_t* fsm = fsm_create(buf, sizeof(buf), NULL);
fsm_add_state(fsm, IDLE, idle_handler);
fsm_add_state(fsm, RUNNING, running_handler);
fsm_add_state(fsm, ERROR, error_handler);
```

## 运行状态机
周期性使用fsm_step函数运行状态机，根据当前状态及事件触发相应的状态句柄函数
```c
while (1) {
	fsm_step(fsm);
}
```

## 使用计时功能
如果需要使用计时功能，需要在创建状态机实例时传入时间获取函数。
```c
uint32_t tick_handler() {
	// 获取当前时间(推荐单位为ms)
}

fsm_t* fsm = fsm_create(buf, sizeof(buf), tick_handler());
```

传入正确的函数后，在状态运行时可使用fsm_get_time函数获取当前状态持续时间。
```c
void running_handler(fsm_t* fsm, fsm_event event) {
    uint32_t time = fsm_get_time(fsm); //获取当前状态持续时间
    // 处理RUNNING状态
}
```

## 还是不理解？
请查看examples目录，该目录准备了一些使用示例以供学习。
示例均在Linux下测试编译通过，如需使用示例文件到其他平台请自行适配。

# 集成到你的项目
该框架支持两种集成方式：
1. 直接将src和include文件夹拷贝到你的项目目录下进行集成
2. 使用cmake构建系统，在CMakeLists.txt中添加
```cmake
add_subdirectory(LRain_FSM)

# 在add_executable之后添加
target_link_libraries(${YOUR_PROJECT_NAME} LRain_FSM)
```

# 许可证
LRain_FSM遵循MIT许可证，你可以在遵守该许可证的前提下自由使用该框架。
