
#include "stm32f1xx_hal.h"
#include "stdbool.h"

#define IS_PACKED_RCV()  EspPackRcv(50)
#define CR_LF            2

//COMANDOS AT

#define AT                     "AT\r\n" 
#define AT_CWMODE_ST_MODE      "AT+CWMODE=1\r\n"
#define AT_CWMODE_AP_MODE      "AT+CWMODE=2\r\n"
#define AT_CWMODE_ST_AP_MODE   "AT+CWMODE=3\r\n"
#define AT_CIPMUX_SING         "AT+CIPMUX=0\r\n"
#define AT_CIPMUX_MULT         "AT+CIPMUX=1\r\n"

//LONGITUD AT

#define AT_LEN           2 + CR_LF
#define AT_CWMODE_LEN   11 + CR_LF
#define AT_CIPMUX_LEN   11 + CR_LF  

//MACROS AT

#define EspSendAT()  EspWriteCmd(AT, AT_LEN)

typedef enum{
    STATION_MODE = 1,
    SOFTAP_MODE,
    ST_AP
} mode_t;

HAL_StatusTypeDef EspInit(UART_HandleTypeDef * huart);
void EspRead(void);
void EspWrite(const char * espAtCmd, uint8_t espLen);
void EspSaveTick(void);
bool EspPackRcv(uint16_t espTime);
HAL_StatusTypeDef EspReadPack(const char * espCmp);
HAL_StatusTypeDef EspIsPacketOk(void);
HAL_StatusTypeDef EspWriteCmd(const char * espAtCmd, uint8_t espLen);
HAL_StatusTypeDef EspWriteT(const char * espAtCmd, uint16_t espTime, uint8_t espLen);
void EspSetMode(mode_t espMode);
HAL_StatusTypeDef EspConnect(const char * espNet, const char * espPass);
uint8_t * EspCmpStr(uint8_t * s1, uint8_t * s2);
void EspFlush(void);
//uint8_t data2[] = "HTTP/1.0 200 OK\r\nContent-Type:text/html\r\n\r\n\n<html>\n<head></head>\n<body>Hugo Puto.l.</body>\n</html>";

/* FIN */