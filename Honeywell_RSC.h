#ifndef HONEYWELL_RSC_H
#define HONEYWELL_RSC_H

#include <Arduino.h>
#include <SPI.h>

#include "rsc_regs.h"

class Honeywell_RSC {
public:
  Honeywell_RSC(int drdy_pin, int cs_ee_pin, int cs_adc_pin);

  // read from or write to an address 1 byte
  void Honeywell_RSC::eeprom_read(uint16_t address, uint8_t len, uint8_t *buf);

  // chip selection
  void select_eeprom();
  void deselect_eeprom();
  void select_adc();
  void deselect_adc();
  
  // read constants from EEPROM
  void get_catalog_listing();
  void get_serial_number();
  void get_pressure_range();
  void get_pressure_minimum();
  void get_pressure_unit();
  void get_pressure_type();

  // getter
  char* catalog_listing() {return _catalog_listing;}
  char* serial_number() {return _serial_number;}
  float pressure_range() {return _pressure_range;}
  float pressure_minimum() {return _pressure_minimum;}
  PRESSURE_U pressure_unit() {return _pressure_unit;}
  PRESSURE_T pressure_type() {return _pressure_type;}

private:
  int _drdy_pin;
  int _cs_ee_pin;
  int _cs_adc_pin;

  char _catalog_listing[RSC_SENSOR_NAME_LEN];
  char _serial_number[RSC_SENSOR_NUMBER_LEN];
  float _pressure_range;
  float _pressure_minimum;
  PRESSURE_U _pressure_unit;
  PRESSURE_T _pressure_type;
};

#endif // HONEYWELL_RSC_H
