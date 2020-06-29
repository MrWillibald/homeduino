#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void bme_setup() {
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println(F("Could not find a valid BME280 sensor, check wiring, address, sensor ID!"));
        Serial.print(F("SensorID was: 0x")); Serial.println(bme.sensorID(),16);
        Serial.print(F("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n"));
        Serial.print(F("   ID of 0x56-0x58 represents a BMP 280,\n"));
        Serial.print(F("        ID of 0x60 represents a BME 280.\n"));
        Serial.print(F("        ID of 0x61 represents a BME 680.\n"));
        while (1) delay(10);
    }
}

void bme_command() {
    char* arg = sCmd.next();
    if(arg != NULL) {
        argument_error();
        return;
    }

    Serial.print(F("ACK "));
    Serial.print(bme.readTemperature(), 1);
    Serial.write(' ');
    Serial.print(bme.readHumidity(), 1);
    Serial.write(' ');
    Serial.print(bme.readPressure() / 100.0F, 1);
    Serial.write(' ');
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA), 1);
    Serial.print(F("\r\n"));
}
