#ifndef SENSOR_H
#define SENSOR_H
#include "LoRaWan_APP.h"
#include "BMP280.h"
#include "HDC1080.h"

struct hdc1080_data
{
    float temperature;
    float humidity;
};

struct bmp280_data
{
    float bmp280_internal_temperature;
    float pressure;
};

extern struct hdc1080_data hdc1080_result;
extern struct bmp280_data bmp280_result;

extern BMP280 bmp;
extern HDC1080 hdc1080;
bool hdc1080_fetch(void);
bool bmp280_fetch(void);
void fetchSensorData(void);
#endif