#ifndef __LRain_FSM_H__
#define __LRain_FSM_H__

#include <stdint.h>
#include "FSM_conf.h"


struct fsm;
typedef struct fsm fsm_t;
typedef struct fsm_state fsm_state_t;

typedef enum fsm_event
{
	FSM_EVENT_INIT = 0,
	FSM_EVENT_DURING,
	FSM_EVENT_EXIT
}fsm_event;

struct fsm_state
{
	uint16_t name;
	uint16_t (*handler)(fsm_t*, fsm_event);		//状态处理函数
};

struct fsm
{
	uint32_t start_time;
	uint32_t (*tick_handler)(void);	//用户实现该时间戳获取函数
	uint16_t current_state;
	uint16_t next_state;
	uint16_t state_max_count;
	fsm_state_t state[0];
};




fsm_t* fsm_create(void* buf, uint32_t size, uint32_t (*tick_handler)(void));
int32_t fsm_add_state(fsm_t* fsm, uint16_t name, uint16_t (*handler)(fsm_t*, fsm_event));
int32_t fsm_step(fsm_t* fsm);




/**
 * 部分实用工具函数，通过内联调用的方式植入调用者函数
 */

/**
 * @brief	获取当前运行状态
 * @param[in]	fsm	状态机实例
 * @retval	当前运行状态
 */
static inline uint16_t fsm_get_state(fsm_t* fsm)
{
	return fsm->state[fsm->current_state].name;
}

/**
 * @brief	获取当前运行状态持续时间
 * @param[in]	fsm	状态机实例
 * @retval	当前运行状态持续时间
 * @attention	该函数仅当用户实现了时间戳获取函数fsm_get_tick()时有效
 */
static inline uint32_t fsm_get_time(fsm_t* fsm)
{
	return fsm_get_tick() - fsm->start_time;
}





#endif /* __LRain_FSM_H__ */