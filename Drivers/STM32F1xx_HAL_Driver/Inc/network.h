
#include "stm32f1xx_hal.h"
#include "syslog.h"

#ifdef ENABLE_SYSLOG
void NetworkInit(UART_HandleTypeDef * huart1, UART_HandleTypeDef * huart2);
#else
void NetworkInit(UART_HandleTypeDef * huart1);
#endif

void NetworkRead(void);