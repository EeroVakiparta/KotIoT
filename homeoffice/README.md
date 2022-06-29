## List of hardware

AHT20+BMP280 € 1,74

 3A MINI DC-DC STEP DOWN CONVERTER € 0,30

 ESP32 € 4,52

 haptic DC motor 0.54 €
 Rated voltage: 5.0VDC
 Operating voltage: 3.0 - 5.3VDC 
 Rated speed: 9000 rpm minimum 
 Rated current: up to 60 mA 
 Starting current: up to 90 mA 
 Starting voltage: DC3.7V


 ## Tasks by priority and core

 ### CORE 0
 1. WIFI, as not yelding for wifi will cause errors.
 2. 

 ### CORE 1
 1. All direct interaction for smooth operation. Example: Resource-Button.
 2. Non-direct interaction: Example: Resource-Motion sensor.
 3. Scedulers for sensors. As exact time of measures is not importan reading from sensors are set to IDLE priority. Example: CO2 sensor.

## External libraries

- SparkFun_SGP30_Arduino_Library
    - For interfacing the air quality sensor (CO2 & VOC).
- Wire
    - For communicating with I2C devices.
- SPI
    - For communicating with SPI devices.
- Adafruit_BMP280
    - For reading from barometric pressure sensor.
- kotiot_homeoffice
    - For cleaner code, storing trivial functions in separate library file.

### TODO

https://www.yr.no/en/forecast/daily-table/2-640999/Finland/Satakunta/Pori/Pori
