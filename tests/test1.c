#include <stdio.h>
#include "LRain_FSM.h"

uint8_t buf[1024];

// 定义一些状态处理函数
uint16_t init_handler(fsm_t* fsm, fsm_event event)
{
    (void)fsm;
    switch (event) {
        case FSM_EVENT_INIT:
            printf("Init state: INIT\n");
            break;
        case FSM_EVENT_DURING:
            printf("Init state: DURING -> staying in init\n");
            break;
        case FSM_EVENT_EXIT:
            printf("Init state: EXIT\n");
            break;
    }
    return 0; // 保持在状态0
}

uint16_t work_handler(fsm_t* fsm, fsm_event event)
{
    (void)fsm;
    switch (event) {
        case FSM_EVENT_INIT:
            printf("Work state: INIT\n");
            break;
        case FSM_EVENT_DURING:
            printf("Work state: DURING -> going to init\n");
            return 0; // 切换到状态0
        case FSM_EVENT_EXIT:
            printf("Work state: EXIT\n");
            break;
    }
    return 1; // 保持在状态1
}

int main()
{
    printf("Testing FSM library...\n");
    
    fsm_t* fsm = fsm_create(buf, sizeof(buf), NULL);
    if (fsm == NULL) {
        printf("Failed to create FSM\n");
        return -1;
    }
    
    // 添加状态
    if (fsm_add_state(fsm, 0, init_handler) != 0) {
        printf("Failed to add state 0\n");
        return -1;
    }
    if (fsm_add_state(fsm, 1, work_handler) != 0) {
        printf("Failed to add state 1\n");
        return -1;
    }
    
    // 初始状态为0
    fsm->current_state = 0;
    fsm->next_state = 0;
    
    // 运行几步
    for (int i = 0; i < 5; i++) {
        printf("Step %d:\n", i);
        int32_t result = fsm_step(fsm);
        if (result != 0) {
            printf("fsm_step returned error: %d\n", result);
            break;
        }
        printf("Current state: %u\n", fsm_get_state(fsm));
    }
    
    printf("Test completed.\n");
    return 0;
}
