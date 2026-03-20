#include "LRain_FSM.h"
#include "FSM_conf.h"
#include <stddef.h>

/**
 * @brief	创建一个新状态机实例
 * @param[in]	buf	指向状态机存储空间的指针
 * @param[in]	size	状态机存储空间的大小
 * @param[in]	tick_handler	时间获取回调函数(不使用该功能可填NULL)
 * @return	指向新创建的状态机实例的指针,创建失败返回NULL
 */
fsm_t* fsm_create(void* buf, uint32_t size, uint32_t (*tick_handler)(void))
{
	if (buf == NULL) return NULL;
	if (size < sizeof(fsm_t)) return 0;
	fsm_t* ret = buf;
	ret->state_max_count = (size - sizeof(fsm_t)) / sizeof(fsm_state_t);

	ret->tick_handler = tick_handler;
	ret->current_state = 0;
	ret->next_state = 0;
	ret->start_time = 0;
	return ret;
}

/**
 * @brief	状态机运行函数
 * @param	fsm	需要运行的状态机实例
 * @retval	0	运行正常
 * @retval	-1	给定的状态机实例无效
 * @retval	-2	跳转状态超限
 * @retval	-3	运行状态句柄函数无效
 * @retval	-4	下一个状态的处理函数无效
 */
int32_t fsm_step(fsm_t* fsm)
{
	if (fsm == NULL) return -1;
	
	// 检查是否需要切换状态
	if (fsm->next_state != fsm->current_state) {
		// 验证下一个状态是否有效
		if (fsm->next_state > fsm->state_max_count) return -2;
		fsm_state_t* next_state = &fsm->state[fsm->next_state];
		if (next_state->handler == NULL) return -4;
		
		// 调用当前状态的退出处理
		fsm_state_t* current_state = &fsm->state[fsm->current_state];
		if (current_state->handler != NULL) {
			current_state->handler(fsm, FSM_EVENT_EXIT);
		}
		
		// 调用下一个状态的初始化处理
		next_state->handler(fsm, FSM_EVENT_INIT);
		
		// 更新当前状态
		fsm->current_state = fsm->next_state;
		
		// 重置开始时间
		if (fsm->tick_handler != NULL) {
			fsm->start_time = fsm->tick_handler();
		}
	}
	
	// 获取当前状态
	fsm_state_t* current_state = &fsm->state[fsm->current_state];
	
	// 检查当前状态的处理函数
	if (current_state->handler == NULL) return -3;
	
	// 运行当前状态的处理函数
	uint16_t next_state_id = current_state->handler(fsm, FSM_EVENT_DURING);
	
	// 验证返回的状态ID
	if (next_state_id > fsm->state_max_count) return -2;
	
	// 设置下一个状态
	fsm->next_state = next_state_id;
	
	return 0;
}

/**
 * @brief	添加一个状态到给定的状态机中
 * @param[in]	fsm		给定的状态机实例
 * @param[in]	state_id	状态ID
 * @param[in]	handler		状态的处理函数
 * @retval	0		添加成功
 * @retval	-1		给定的状态机实例无效
 * @retval	-2		给定的状态句柄函数无效
 * @retval	-3		给定的状态名称超出容纳最大值
 */
int32_t fsm_add_state(fsm_t* fsm, uint16_t state_id, uint16_t (*handler)(fsm_t*, fsm_event))
{
	if (fsm == NULL) return -1;
	if (handler == NULL) return -2;
	if (state_id > fsm->state_max_count) return -3;
	fsm_state_t* state = &fsm->state[state_id];
	state->name = state_id;
	state->handler = handler;
	return 0;
}
