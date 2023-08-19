
#include "stm32f1xx_hal.h"

/* Habilitar Syslog para deteccion de errores*/
#define ENABLE_SYSLOG

#ifdef ENABLE_SYSLOG

#define SysWrite(x) Syslog_Write(x)
void SysInit(UART_HandleTypeDef * huart);
void Syslog_Write(const char * msg);

#else
#define SysWrite(x)

#endif

/* FIN */