
#include "stm32f1xx_hal.h"
#include "stdint.h"

#define TcpSetSing()  EspWriteT(AT_CIPMUX_SING,200,AT_CIPMUX_LEN) 
#define TcpSetMult()  EspWriteT(AT_CIPMUX_MULT,200,AT_CIPMUX_LEN) 

typedef struct{
    uint32_t localIp;
}tcp_t;

HAL_StatusTypeDef TcpInit(void);
;