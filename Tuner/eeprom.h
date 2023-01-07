#pragma once
#ifndef _EEPROM_H
#define _EEPROM_H

#include "global.h"

#define EEPROM_ADDR 0xA0

bool eeprom_check(void);
uint8_t eeprom_read_byte(uint16_t p);
uint16_t eeprom_read_word(uint16_t p);
void eeprom_write_byte(uint16_t p, uint8_t val);
void eeprom_write_word(uint16_t p, uint16_t val);

#endif /* _EEPROM_H */