//
// Created by yuri on 21. 8. 5..
//

#include "am2302.h"
#include "main.h"


/* 외부에서 구현 필요함 */
extern void delay_us(uint16_t us);
extern void startUsCounter();
extern uint32_t getUsCounter();

const THdata AM2302_SENSOR_ERROR = {0xFFFF,0xFFFF};


THdata am2302_getSensorData(){
    THdata sensorVal;
    uint32_t rawRxData = 0;
    uint8_t checksumData = 0;
    HAL_GPIO_WritePin(TH_pin_GPIO_Port,TH_pin_Pin,GPIO_PIN_RESET); //센서 요청
    delay_us(2000);
    HAL_GPIO_WritePin(TH_pin_GPIO_Port,TH_pin_Pin,GPIO_PIN_SET); //센서에 요청 후 gpio 라인을 릴리즈 (오픈 드레인 출력)


    uint32_t startTime = HAL_GetTick(); // 센서 타임아웃 체크용 타임스탬프

    //센서 응답 대기
    while(HAL_GPIO_ReadPin(TH_pin_GPIO_Port,TH_pin_Pin) != GPIO_PIN_RESET){if(HAL_GetTick() - startTime > 1000){return AM2302_SENSOR_ERROR;}}
    while(HAL_GPIO_ReadPin(TH_pin_GPIO_Port,TH_pin_Pin) != GPIO_PIN_SET){if(HAL_GetTick() - startTime > 1000){return AM2302_SENSOR_ERROR;}}

    //첫번째 비트 전송 시작까지 대기
    while(HAL_GPIO_ReadPin(TH_pin_GPIO_Port,TH_pin_Pin) != GPIO_PIN_RESET){if(HAL_GetTick() - startTime > 1000){return AM2302_SENSOR_ERROR;}}

    for(int i=39 ; i >=0 ;i--){
        while(HAL_GPIO_ReadPin(TH_pin_GPIO_Port,TH_pin_Pin) != GPIO_PIN_SET){if(HAL_GetTick() - startTime > 1000){return AM2302_SENSOR_ERROR;}}
        startUsCounter();
        while(HAL_GPIO_ReadPin(TH_pin_GPIO_Port,TH_pin_Pin) != GPIO_PIN_RESET){if(HAL_GetTick() - startTime > 1000){return AM2302_SENSOR_ERROR;}}
        unsigned int time = getUsCounter();
        if(time > 50){
            if(i >= 8){
                rawRxData |= 1 << (i-8);
            }
            else{
                checksumData |= 1 << i;
            }

        }
    }

    /* 체크섬 검증 */
    uint8_t* checkPtr = (uint8_t *) &rawRxData;
    uint8_t expected_checksum_Data = 0;
    for(int i=0;i<4;i++){
        expected_checksum_Data += *checkPtr;
        checkPtr++;
    }
    if(checksumData != expected_checksum_Data){
        return AM2302_SENSOR_ERROR;
    }


    sensorVal.rh = rawRxData >>16;
    sensorVal.temp = (int32_t)(rawRxData & 0x7FFF);
    if(rawRxData & 0x8000){
        sensorVal.temp *= -1;
    }

    return sensorVal;
}
