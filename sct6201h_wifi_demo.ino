#include "sct62_bsp.h"
#include "sensor.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Base64.h> // Include a base64 library if available

RTC_DATA_ATTR int bootCount = 0;
uint64_t Sleep_uSec = 60 * 1000000; // unit : uSec

const char *ssid = "yourWiFiSsid";         // replace your WiFi SSID
const char *password = "yourWiFiPassword"; // replace your WiFi password
const char *mqtt_server = "192.168.1.105"; // replace your MQTT broker IP
const int mqtt_port = 1883;                // Standard MQTT port

float getBatVolt();
uint8_t GetBatteryLevel(void);

String base64Encoded;

static void prepareTxFrame(uint8_t port)
{

  appDataSize = sizeof(hdc1080_data) + sizeof(bmp280_data) + 1;
  Serial.println("Fetch data Done");
  Serial.printf("T=%.2f degC, Pressure=%.2f hPa\n", bmp280_result.bmp280_internal_temperature, bmp280_result.pressure);
  Serial.printf("T=%.2f degC, Humidity=%.2f %\n", hdc1080_result.temperature, hdc1080_result.humidity);

  // Prepare appData as before
  appData[0] = GetBatteryLevel();
  memcpy(&appData[1], &hdc1080_result, sizeof(hdc1080_data));
  memcpy(&appData[sizeof(hdc1080_data) + 1], &bmp280_result, sizeof(bmp280_data));

  // Base64 Encoding
  base64Encoded = base64::encode(appData, appDataSize);

  // Use or transmit the Base64 encoded string
  Serial.println(base64Encoded);

  Serial.flush();
}

void setup_wifi()
{
  delay(10);
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client"))
    { // "ESP32Client" is the MQTT client ID
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}
void setup()
{
  Serial.begin(115200);
  Mcu.begin();
  print_wakeup_reason();

  fetchSensorData();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  // set sleep mode
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  Sleep_uSec += randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
  // setting wakeup time
  esp_sleep_enable_timer_wakeup(Sleep_uSec); // uSecond

  bootCount++;
  Serial.printf("this is boot number %d\n", bootCount);
  delay(500);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  prepareTxFrame(0);
  // Publish a message
  // client.publish("esp32/test", jsonData.c_str());

  client.publish("esp32/test", base64Encoded.c_str());
  delay(500); // Send a message every 5 seconds
  esp_deep_sleep_start();
}

float getBatVolt()
{
  uint32_t sum = 0;
  uint32_t test_min = 695;
  uint32_t test_max = 1030;
  for (size_t i = 0; i < 16; i++)
  {
    sum += analogRead(2);
    delay(10);
  }
  float avg = (float)(sum >> 4) / 4095 * 2400;
  Serial.print("avg");
  Serial.println(avg);
  return ((avg - test_min) * (4.2 - 3) / (test_max - test_min) + 3);
}

uint8_t GetBatteryLevel(void)
{
  const float maxBattery = 4.2;
  const float minBattery = 3.0;
  const float batVolt = getBatVolt();
  const float batVoltage = fmax(minBattery, fmin(maxBattery, batVolt));
  uint8_t batLevel = BAT_LEVEL_EMPTY + ((batVoltage - minBattery) / (maxBattery - minBattery)) * (BAT_LEVEL_FULL - BAT_LEVEL_EMPTY);
  if (batVolt > 4.2)
  {
    batLevel = 255;
  }
  if (batVolt < 3.00)
  {
    batLevel = 0;
  }
  Serial.print("{");
  Serial.println(batVoltage);
  Serial.print(batLevel);
  Serial.println("}");
  return batLevel;
}
