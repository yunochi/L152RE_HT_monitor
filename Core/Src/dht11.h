//
// Created by yuri on 21. 8. 5..
//

#ifndef ARCDETECTOR_DHT11_H
#define ARCDETECTOR_DHT11_H
#include "stdint.h"


struct {
    uint16_t rh;
    int32_t temp;
}typedef THdata;

extern const THdata DHT11_SENSOR_ERROR;


THdata dht11_getSensorData();


#endif //ARCDETECTOR_DHT11_H
