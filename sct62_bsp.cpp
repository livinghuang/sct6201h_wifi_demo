#include "sct62_bsp.h"
#include "Arduino.h"
void power_On_Sensor_Bus()
{
    pinMode(pVext, OUTPUT);
    // according the I2C bus collision with the JTAG/USB pin , it is internal pull up, we need to set it high before turn on the Vext to avoid system restart
    pinMode(pSDA, OUTPUT);
    pinMode(pSCL, OUTPUT);
    digitalWrite(pSDA, HIGH);
    digitalWrite(pSCL, HIGH);
    // Serial.println("I2C ON");
    delay(15);
    digitalWrite(pVext, LOW);
    // Serial.println("Vext ON");
}

void power_Off_Sensor_Bus()
{
    pinMode(pVext, OUTPUT);
    // according the I2C bus collision with the JTAG/USB pin , it is internal pull down, we need to set it low before turn off the Vext to keep the Vext level in low.
    pinMode(pSDA, OUTPUT);
    pinMode(pSCL, OUTPUT);
    digitalWrite(pSDA, LOW);
    digitalWrite(pSCL, LOW);
    // Serial.println("I2C OFF");
    delay(15);
    digitalWrite(pVext, HIGH);
    // Serial.println("Vext OFF");
}