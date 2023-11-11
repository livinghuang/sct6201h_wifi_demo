#include "sensor.h"
#include "Arduino.h"

BMP280 bmp;
HDC1080 hdc1080;
struct hdc1080_data hdc1080_result;
struct bmp280_data bmp280_result;

void fetchSensorData(void)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        power_On_Sensor_Bus();
        delay(15);
        hdc1080_fetch();
        delay(10);
        power_Off_Sensor_Bus();
        delay(15);
    }
    for (i = 0; i < 5; i++)
    {
        power_On_Sensor_Bus();
        delay(10);
        bmp280_fetch();
        // delay(10);
        power_Off_Sensor_Bus();
        delay(5);
    }
}

bool hdc1080_fetch(void)
{

    if (!hdc1080.begin())
    {
        return 0;
    }
    float temp = hdc1080.readTemperature();
    float Humidity = hdc1080.readHumidity();
    temp = hdc1080.readTemperature();
    Humidity = hdc1080.readHumidity();
    hdc1080.end();
    // Serial.printf("T=%.2f degC, Humidity=%.2f %\n", temp, Humidity);
    hdc1080_result.temperature = temp;
    hdc1080_result.humidity = Humidity;
    return 1;
}

bool bmp280_fetch(void)
{
    if (!bmp.begin())
    {
        return 0;
    }
    delay(50);
    bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                    BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    BMP280::FILTER_X16,      /* Filtering. */
                    BMP280::STANDBY_MS_500); /* Standby time. */
    delay(50);
    float temp = bmp.readTemperature();
    float Pressure = (float)bmp.readPressure() / 100.0;
    delay(100);
    int c = 0;
    while ((temp < -50) || (Pressure > 1100) || (Pressure < 500))
    {
        bmp.putBMP280ToSleep();
        delay(10);
        bmp.end();
        Serial.println("BMP ERROR");
        Serial.flush();
        delay(100);
        bmp.begin();
        delay(100);
        bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                        BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        BMP280::FILTER_X16,      /* Filtering. */
                        BMP280::STANDBY_MS_500); /* Standby time. */
        temp = bmp.readTemperature();
        Pressure = (float)bmp.readPressure() / 100.0;
        c++;
        if (c > 3)
        {
            return false;
        }
    }
    bmp.putBMP280ToSleep();
    delay(100);
    bmp.end();
    // Serial.printf("T=%.2f degC, Pressure=%.2f hPa\n", temp, Pressure);
    bmp280_result.bmp280_internal_temperature = temp;
    bmp280_result.pressure = Pressure;
    return true;
}