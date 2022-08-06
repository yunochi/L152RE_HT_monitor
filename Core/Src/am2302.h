//
// Created by yuri on 21. 8. 5..
//

#ifndef ARCDETECTOR_AM2302_H
#define ARCDETECTOR_AM2302_H
#include "stdint.h"


struct {
    uint16_t rh;
    int32_t temp;
}typedef THdata;

extern const THdata AM2302_SENSOR_ERROR;


THdata am2302_getSensorData();


#endif //ARCDETECTOR_AM2302_H
