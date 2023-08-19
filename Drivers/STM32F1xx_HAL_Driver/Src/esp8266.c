
#include "esp8266_def.h"
#include "syslog.h"
#include "string.h"

UART_HandleTypeDef * esp;

extern UART_HandleTypeDef huart1;

uint8_t espBuf[512];
uint32_t espTick;

HAL_StatusTypeDef EspInit(UART_HandleTypeDef * huart){
    uint8_t espTimeOut = 0;
    if(huart == NULL){
        SysWrite("No existe un modulo de comunicacion\r\n");
        return HAL_ERROR;
    }
    esp = huart;
    espBuf[511] = 0;
    esp->pRxBuffPtr = espBuf;
    esp->RxXferSize = 512;
    esp->RxXferCount = 0;
    esp->RxState = HAL_UART_STATE_READY;
    EspFlush();
    espTick = HAL_GetTick();
    SET_BIT(esp->Instance->CR1,USART_CR1_RE);
    SET_BIT(esp->Instance->CR1,USART_CR1_RXNEIE);    
    HAL_Delay(2);
    do{
        if(EspSendAT() == HAL_OK){
            SysWrite("ESP INIT\r\n");
            return HAL_OK;  
        } 
        HAL_Delay(5);
    }while(++espTimeOut < 10);
    return HAL_ERROR;
}

void EspRead(void){
    esp->RxState = HAL_UART_STATE_BUSY;
    *esp->pRxBuffPtr++ = (uint8_t)READ_REG(esp->Instance->DR & 0x00FF);
    if(esp->RxXferCount++ == 510){
        esp->RxXferCount = 0;
        esp->pRxBuffPtr = espBuf;
    }
    EspSaveTick();
}

void EspWrite(const char * espAtCmd, uint8_t espLen){
    HAL_UART_Transmit(esp,(uint8_t *)espAtCmd,espLen,100);
}

void EspSaveTick(void){
    espTick = HAL_GetTick();
}

bool EspPackRcv(uint16_t espTime){
    if((HAL_GetTick() - espTick) > espTime) return true;
    return false;
}

HAL_StatusTypeDef EspReadPack(const char * espCmp){
    if(EspCmpStr(espBuf,(uint8_t *)espCmp) != NULL){
        return HAL_OK;
    }
    else return HAL_ERROR;
}

HAL_StatusTypeDef EspIsPacketOk(void){
    if(EspReadPack("OK") == HAL_OK) return HAL_OK;
    else return HAL_ERROR;
}

HAL_StatusTypeDef EspWriteCmd(const char * espAtCmd, uint8_t espLen){
    EspWrite(espAtCmd, espLen);
    while(!IS_PACKED_RCV());
    SysWrite((char *)espBuf);
    if(EspIsPacketOk() == HAL_OK){
        EspFlush();
        return HAL_OK;
    }
    else{
        EspFlush();
        return HAL_ERROR;
    }    
}

HAL_StatusTypeDef EspWriteT(const char * espAtCmd, uint16_t espTime, uint8_t espLen){ //Para comandos que requieren mas tiempo de respuesta
    EspWrite(espAtCmd, espLen);
    while(!EspPackRcv(espTime));
    SysWrite((char *)espBuf);
    if(EspIsPacketOk() == HAL_OK){
        EspFlush();
        return HAL_OK;
    }
    else{
        EspFlush();
        return HAL_ERROR;
    }        
}

uint8_t * EspCmpStr(uint8_t * s1, uint8_t * s2){
    uint8_t strCnt;
    uint8_t * c2;
    if(!*s2) return NULL;   
    while(*s1){
        strCnt = 0;
        c2 = s2;
        while(*s1++ == *c2){
            strCnt++;
            if(!*++c2) return s1 - strCnt;
        }
    }
    return NULL;
}

void EspFlush(void){
    esp->RxXferCount = 0;
    esp->pRxBuffPtr = espBuf;
    while(*esp->pRxBuffPtr){
        *esp->pRxBuffPtr++ = 0;
    }
    esp->pRxBuffPtr = espBuf;
    esp->RxState = HAL_UART_STATE_READY;
}

/*Funciones relacionadas directamente con comandos at*/

void EspSetMode(mode_t espMode){ //En el caso de envio correcto se responde con un no change
    switch(espMode){
        case STATION_MODE:
            EspWrite(AT_CWMODE_ST_MODE, AT_CWMODE_LEN);
            break;
        case SOFTAP_MODE:
            EspWrite(AT_CWMODE_AP_MODE, AT_CWMODE_LEN);
            break;
        case ST_AP:
            EspWrite(AT_CWMODE_ST_AP_MODE, AT_CWMODE_LEN);
            break;
    }
}

HAL_StatusTypeDef EspConnect(const char * espNet, const char * espPass){
    EspWrite("AT+CWJAP=\"", 10);
    EspWrite(espNet, strlen(espNet));
    EspWrite("\",\"", 3);
    EspWrite(espPass, strlen(espPass));
    EspWrite("\"\r\n", 3);
    while(!EspPackRcv(1000));
    SysWrite((char *)espBuf);   
        if(EspIsPacketOk() == HAL_OK){
        EspFlush();
        return HAL_OK;
    }
    else{
        EspFlush();
        return HAL_ERROR;
    } 
}
/* FIN */