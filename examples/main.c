#include "LRain_FSM.h"
#include <stdio.h>

uint8_t buf[1024];

enum main_state
{
	INIT = 0,
	STATE0,
	STATE1,
	STATE2
};

uint16_t init_handler(fsm_t* fsm, fsm_event event)
{
	if (event == FSM_EVENT_INIT) {
		printf("init in init state is never being run!\n");	//初始化状态的初始化过程永远不会被调用！
		return STATE0;

	} else if (event == FSM_EVENT_DURING) {
		printf("state init changed to state0\n");
		return STATE0;					//这里的返回值为需要跳转的状态

	} else if (event == FSM_EVENT_EXIT) {
		printf("State init exit\n");
		return STATE0;					//这里的返回值不被状态机引擎接收，可以为任意值
	}
	return -1;
}


uint16_t state0_handler(fsm_t* fsm, fsm_event event)
{
	if (event == FSM_EVENT_INIT) {
		printf("State 0 init\n");
		return STATE0;					//这里的返回值不被状态机引擎接收，可以为任意值

	} else if (event == FSM_EVENT_DURING) {
		printf("State 0 during,changed to state1\n");
		return STATE1;					//这里的返回值为需要跳转的状态

	} else if (event == FSM_EVENT_EXIT) {
		printf("State 0 exit\n");
		return STATE0;					//这里的返回值不被状态机引擎接收，可以为任意值
	}
	return -1;
}

uint16_t state1_handler(fsm_t* fsm, fsm_event event)
{
	static int i;						//计数器变量，模拟状态多次重入
	if (event == FSM_EVENT_INIT) {
		printf("State 1 init\n");
		return -1;					//这里的返回值不被状态机引擎接收，可以为任意值

	} else if (event == FSM_EVENT_DURING) {
		printf("State 1 during\n");
		i++;
		printf("i is %d\n", i);
		if (i > 10) {
			printf("ready to change\n");
			return STATE2;				//这里的返回值为需要跳转的状态
		}
		return STATE1;					//这里的返回值为需要跳转的状态

	} else if (event == FSM_EVENT_EXIT) {
		printf("State 1 exit\n");
		return -1;					//这里的返回值不被状态机引擎接收，可以为任意值
	}
	return -1;
}

uint16_t state2_handler(fsm_t* fsm, fsm_event event)
{
	if (event == FSM_EVENT_INIT) {
		printf("State 2 init\n");
		return 0;					//这里的返回值不被状态机引擎接收，可以为任意值

	} else if (event == FSM_EVENT_DURING) {
		printf("State 2 during,accidently changed to invalid state\n");
		return -1;					//这里的返回值为需要跳转的状态

	} else if (event == FSM_EVENT_EXIT) {
		printf("State 2 exit\n");
		return 0;					//这里的返回值不被状态机引擎接收，可以为任意值
	}
	return -1;
}


int main()
{
	fsm_t* fsm = fsm_create(buf, sizeof(buf));
	fsm_add_state(fsm, INIT, init_handler);
	fsm_add_state(fsm, STATE0, state0_handler);
	fsm_add_state(fsm, STATE1, state1_handler);
	fsm_add_state(fsm, STATE2, state2_handler);

	while(fsm_step(fsm) >= 0);
	return 0;
}