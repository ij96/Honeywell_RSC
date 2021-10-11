/*
  circuit:
  pin name     pin number on Arduino
    DRDY         6
    CS_EE        7
    CS_ADC       8
    MOSI (DIN)   11
    MISO (DOUT)  12
    SCK          13
*/

#include "Honeywell_RSC.h"

// pins used for the connection with the sensor
// the other you need are controlled by the SPI library):
#define DRDY_PIN      6
#define CS_EE_PIN     7
#define CS_ADC_PIN    8

// create Honeywell_RSC instance
Honeywell_RSC rsc(
  DRDY_PIN,   // data ready
  CS_EE_PIN,  // chip select EEPROM (active-low)
  CS_ADC_PIN  // chip select ADC (active-low)
);

void setup() {
  // open serial communication
  Serial.begin(9600);

  // open SPI communication
  SPI.begin();

  // allowtime to setup SPI
  delay(5);

  // initialse pressure sensor
  rsc.init();

  // print sensor information
  Serial.println();
  Serial.print(F("catalog listing:\t"));
  Serial.println(*rsc.catalog_listing());
  Serial.print(F("serial number:\t\t"));
  Serial.println(*rsc.serial_number());
  Serial.print(F("pressure range:\t\t"));
  Serial.println(rsc.pressure_range());
  Serial.print(F("pressure minimum:\t"));
  Serial.println(rsc.pressure_minimum());
  Serial.print(F("pressure unit:\t\t"));
  Serial.println(*rsc.pressure_unit_name());
  Serial.print(F("pressure type:\t\t"));
  Serial.println(*rsc.pressure_type_name());
  Serial.println();

  // measure temperature
  Serial.print(F("temperature: "));
  Serial.println(rsc.get_temperature());
  Serial.println();
  delay(5);
}

void loop() {
  // measure pressure
  Serial.print(F("pressure: "));
  Serial.println(rsc.get_pressure());
  delay(1000);
}
