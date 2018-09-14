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

  // read and print constants that are stored in EEPROM
  rsc.get_catalog_listing();
  rsc.get_serial_number();
  rsc.get_pressure_range();
  rsc.get_pressure_minimum();
  rsc.get_pressure_unit();
  rsc.get_pressure_type();

  Serial.print("catalog listing: ");
  Serial.println(rsc.catalog_listing());
  Serial.print("serial number: ");
  Serial.println(rsc.serial_number());
  Serial.print("pressure range: ");
  Serial.println(rsc.pressure_range());
  Serial.print("pressure minimum: ");
  Serial.println(rsc.pressure_minimum());
  Serial.print("pressure unit: ");
  Serial.println(rsc.pressure_unit());
  Serial.print("pressure type: ");
  Serial.println(rsc.pressure_type());
}

void loop() {

}
