#include <stdio.h>
#include "LRain_FSM.h"


uint8_t buf[1024];


int main()
{
	fsm_t* fsm = fsm_create(buf, sizeof(buf), NULL);

}