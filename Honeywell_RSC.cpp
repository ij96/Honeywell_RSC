#include "Honeywell_RSC.h"

Honeywell_RSC::Honeywell_RSC(int drdy_pin, int cs_ee_pin, int cs_adc_pin) {
  _drdy_pin = drdy_pin;
  _cs_ee_pin = cs_ee_pin;
  _cs_adc_pin = cs_adc_pin;

  pinMode(_drdy_pin, INPUT);
  pinMode(_cs_ee_pin, OUTPUT);
  pinMode(_cs_adc_pin, OUTPUT);

  // deselect both EEPROM and ADC
  digitalWrite(_cs_ee_pin, HIGH);
  digitalWrite(_cs_adc_pin, HIGH);
}

void Honeywell_RSC::select_eeprom() {
  digitalWrite(_cs_ee_pin, LOW);
}

void Honeywell_RSC::deselect_eeprom() {
  digitalWrite(_cs_ee_pin, HIGH);
}

void Honeywell_RSC::eeprom_read(uint16_t address, uint8_t len, uint8_t *buf) {
  // generate command (refer to sensor datasheet section 2.2)
  uint16_t command;
  command = RSC_READ_EEPROM_INSTRUCTION | ((address & RSC_EEPROM_ADDRESS_9TH_BIT_MASK) >> 5);
  command = (command << 8) | (address & 0xFF);

  // select EEPROM
  select_eeprom();
  
  // send command
  SPI.transfer16(command);

  // receive results
  // - results are transmitted back after the last bit of the command is sent
  // - to get results, just transfer dummy data, as subsequent bytes will not used by sensor
  for (int i = 0; i < len; i++) {
    buf[i] = SPI.transfer(0x00);
  }
  
  // deselect EEPROM
  // - after command is sent, the sensor will keep sending bytes from EEPROM,
  //   in ascending order of address. Resetting the CS_EE pin at the end of 
  //   the function means that when reading from EEPROM next time, the result
  //   would start at the correct address.
  deselect_eeprom();
}

void Honeywell_RSC::get_catalog_listing() {
  eeprom_read(RSC_CATALOG_LISTING_MSB, RSC_SENSOR_NAME_LEN, _catalog_listing);
}

void Honeywell_RSC::get_serial_number() {
  eeprom_read(RSC_SERIAL_NO_YYYY_MSB, RSC_SENSOR_NUMBER_LEN, _serial_number);
}

void Honeywell_RSC::get_pressure_range() {
  uint8_t buf[RSC_PRESSURE_RANGE_LEN];
  eeprom_read(RSC_PRESSURE_RANGE_LSB, RSC_PRESSURE_RANGE_LEN, buf);
  // convert byte array to float (buf[0] is LSB)
  memcpy(&_pressure_range, &buf, sizeof(_pressure_range));
}

void Honeywell_RSC::get_pressure_minimum() {
  uint8_t buf[RSC_PRESSURE_MINIMUM_LEN];
  eeprom_read(RSC_PRESSURE_MINIMUM_LSB, RSC_PRESSURE_MINIMUM_LEN, buf);
  // convert byte array to float (buf[0] is LSB)
  memcpy(&_pressure_minimum, &buf, sizeof(_pressure_minimum));
}

void Honeywell_RSC::get_pressure_unit() {
  char buf[RSC_PRESSURE_UNIT_LEN] = {0};
  eeprom_read(RSC_PRESSURE_UNIT_MSB, RSC_PRESSURE_UNIT_LEN, buf);
  buf[RSC_PRESSURE_UNIT_LEN-1] = '\0';
  if(buf[RSC_PRESSURE_UNIT_LEN-2] == 'O') {
    _pressure_unit = INH2O;
  } else if(buf[RSC_PRESSURE_UNIT_LEN-2] == 'a') {
    if(buf[RSC_PRESSURE_UNIT_LEN-4] == 'K') {
      _pressure_unit = KPASCAL;
    } else if (buf[RSC_PRESSURE_UNIT_LEN-4] == 'M') {
      _pressure_unit = MPASCAL;
    } else {
      _pressure_unit = PASCAL;
    }
  } else if(buf[RSC_PRESSURE_UNIT_LEN-2] == 'r') {
    if(buf[RSC_PRESSURE_UNIT_LEN-5] == 'm') {
      _pressure_unit = mBAR;
    } else {
      _pressure_unit = BAR;
    }
  } else if(buf[RSC_PRESSURE_UNIT_LEN-2] == 'i') {
    _pressure_unit = PSI;
  }
}

void Honeywell_RSC::get_pressure_type() {
  char buf[RSC_SENSOR_TYPE_LEN];
  eeprom_read(RSC_PRESSURE_REFERENCE, RSC_SENSOR_TYPE_LEN, buf);
  switch (buf[0]) {
    case 'D':
      _pressure_type = DIFFERENTIAL;
      break;
    case 'A':
      _pressure_type = ABSOLUTE;
      break;
    case 'G':
      _pressure_type = GAUGE;
      break;
    default:
      _pressure_type = DIFFERENTIAL;
  }
}

