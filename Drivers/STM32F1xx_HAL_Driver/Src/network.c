
#include "network.h"
#include "esp8266_def.h"
#include "tcp.h"
#include "string.h"

const char mypage[] = "HTTP/1.0 200 OK\r\nContent-Type:text/html\r\n\r\n\n<html>\n<head></head>\n<body>Hola Mundo</body>\n</html>";
extern UART_HandleTypeDef * esp;
extern uint8_t espBuf[256];

#ifdef ENABLE_SYSLOG
void NetworkInit(UART_HandleTypeDef * huart1, UART_HandleTypeDef * huart2){
    SysInit(huart2);
#else
void NetworkInit(UART_HandleTypeDef * huart1){
#endif
    HAL_StatusTypeDef resp;
    SysWrite("Iniciando Esp8266...\r\n");
    HAL_Delay(5000);
    resp = EspInit(huart1);
    while(resp == HAL_OK){
        EspSetMode(ST_AP);
        if(EspConnect("NETLIFE-NAVARRETE","0400336152") != HAL_OK){
            SysWrite("No se ha podido conectar\r\n");
            break;
        } 
        if(TcpInit() != HAL_OK) break;
        SysWrite("NET INIT\r\n");
        return;
    }
    SysWrite("NET ERROR\r\n");
}

void NetworkRead(void){ // Se puede mejorar
    uint8_t * ptr;
    uint8_t idLink[1];
    if(esp->RxState == HAL_UART_STATE_BUSY){
        while(!EspPackRcv(100));
        ptr = EspCmpStr(espBuf,"IPD");
        SysWrite((char *)espBuf);
        if(ptr != NULL){
            idLink[0] = *(ptr + 4);
            EspFlush();
            EspWrite("AT+CIPSEND=",11);
            HAL_UART_Transmit(esp,idLink,1,100);
            EspWrite(",92\r\n",5);
            while(!EspPackRcv(100));
            if(EspCmpStr(espBuf,">")){
                EspWrite(mypage, strlen(mypage));
                HAL_Delay(100);
                EspWrite("AT+CIPCLOSE=",12);
                HAL_UART_Transmit(esp,idLink,1,100);
                EspWrite("\r\n",2);
            }
            SysWrite((char *)espBuf);
            //HAL_Delay(1000); Se puede mejorar
        }
        EspFlush();
    }
}