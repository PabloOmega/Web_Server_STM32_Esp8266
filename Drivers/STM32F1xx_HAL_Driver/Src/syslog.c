
#include "syslog.h"
#include "string.h"

#ifdef ENABLE_SYSLOG

UART_HandleTypeDef * sys;

void SysInit(UART_HandleTypeDef * huart){
    sys = huart;
}

void Syslog_Write(const char * msg){
    HAL_UART_Transmit(sys,(uint8_t *) msg, strlen(msg), 100);
}

#endif