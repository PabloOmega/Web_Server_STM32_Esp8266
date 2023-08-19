
#include "tcp.h"
#include "esp8266_def.h"
#include "syslog.h"

tcp_t tcp;

HAL_StatusTypeDef TcpInit(void){
    tcp.localIp = 0xC0A8649A; //Obtener de AT+CIFSR
    // Crear Tcp server en otra Funcion
    HAL_Delay(100);
    if(TcpSetMult() != HAL_OK){
        SysWrite("Error al habilitar multiple conexion Tcp\r\n");
        return HAL_ERROR;
    }
    HAL_Delay(100);
    EspWriteCmd("AT+CIPSERVER=1,80\r\n",19);   
    SysWrite("TCP INIT\r\n");
    return HAL_OK;
}
;