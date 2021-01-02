# ILI9341 GUI

# ESP32 modules wiring

### SHT30 (through I2C)
|Module PIN|ESP32 PIN|
|:-:|:-:|
|3V3(VCC)|3V3(3.3 V)|
|G(GND)|GND|
|D1(SCL)|D22|
|D2(SDA)|D21|

### RTC DS 1307 (through I2C)

|Module PIN|ESP32 PIN|
|:-:|:-:|
|VCC|3V3|
|GND|GND|
|SCL|D22|
|SDA|D21|

### ILI9341 TFT screen with touch sensor and SD card module(through hardware SPI)

|Module PIN|ESP32 PIN|
|:-:|:-:|
|VCC|VIN(5 V)|
|GND|GND|
|CS|D5|
|RESET|D2|
|DC|D15|
|SDI(MOSI)|D23|
|SCK|D18|
|LED|3V3|
|SDO(MISO)|D19|
|T_CLK|D18|
|T_CS|D14|
|T_DIN|D23|
|T_DO|D19|
|T_IRQ|-|
|SD_CS|D27|
|SD_MOSI|D23|
|SD_MISO|D19|
|SD_SCK|D18|

### Plantower PMS5003 Air Quality Sensor
|Module PIN|ESP32 PIN|
|:-:|:-:|
|1(VCC)|VIN|
|2(GND)|GND|
|3(SET)|D4|
|4(RX)|TX2|
|5(TX)|RX2|
|SET|D4|
