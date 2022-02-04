#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "conf.h"

 /* To keep that of origin of the message*/
#define TEMPERATURESENSOR_PIN 4
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60       /* Time ESP32 will go to sleep (in seconds) */

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire1(TEMPERATURESENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire1);

// TODO: read values with ULP core
RTC_DATA_ATTR float oldTempC1 = 0;
RTC_DATA_ATTR float oldTempC2 = 0;
RTC_DATA_ATTR float oldTempC3 = 0;
RTC_DATA_ATTR float oldTempC4 = 0;
RTC_DATA_ATTR float oldTempC5 = 0;

//MAC Address of the webserver ESP32
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

typedef struct struct_message {
  int id;
  float temp1;
  float temp2;
  float temp3;
  float temp4;
  float temp5;
  int readingId;
} struct_message;

struct_message tempValues;

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

float readDSTemperatureC(int indexi) {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(indexi);

  Serial.println(indexi);
  if (tempC == -127.00) {
    Serial.println("-Failed to read from DS18B20 sensor");
  } else {
    Serial.print("-index temperature: ");
    Serial.println(tempC);
  }
  return tempC;
}

void setup() {
  Serial.begin(115200);

  sensors.begin();
  // does sensors need time to wake up before use?
  delay(100);
  float temperatureC1 = readDSTemperatureC(0);
  float temperatureC2 = readDSTemperatureC(1);
  float temperatureC3 = readDSTemperatureC(2);
  float temperatureC4 = readDSTemperatureC(3);
  float temperatureC5 = readDSTemperatureC(4);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  if ( temperatureC1 != oldTempC1 ||
       temperatureC2 != oldTempC2 ||
       temperatureC3 != oldTempC3 ||
       temperatureC4 != oldTempC4 ||
       temperatureC5 != oldTempC5 ) {
    // Set device as a Wi-Fi Station and set channel
    WiFi.mode(WIFI_STA);

    int32_t channel = getWiFiChannel(ssid);
    WiFi.printDiag(Serial); // Uncomment to verify channel number before
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
    WiFi.printDiag(Serial); // Uncomment to verify channel change after

    //Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    //Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.encrypt = false;

    //Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }

    tempValues.id = SENSOR_ID;
    tempValues.temp1 = temperatureC1;
    tempValues.temp2 = temperatureC2;
    tempValues.temp3 = temperatureC3;
    tempValues.temp4 = temperatureC4;
    tempValues.temp5 = temperatureC5;

    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &tempValues, sizeof(tempValues));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
  
  oldTempC1 = temperatureC1;
  oldTempC2 = temperatureC2;
  oldTempC3 = temperatureC3;
  oldTempC4 = temperatureC4;
  oldTempC5 = temperatureC5;

  delay(5);
  esp_deep_sleep_start();
}

void loop() {

}