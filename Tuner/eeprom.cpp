#include "eeprom.h"

extern I2C_HandleTypeDef hi2c1;

bool eeprom_check(void) {
  uint8_t buf[4];
  buf[0] = 0;
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR, buf, 1, 100);
  return r == HAL_OK;
}

uint8_t eeprom_read_byte(uint16_t p) {

  uint8_t buf[4];
  buf[0] = p & 0xFF;
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR | ((p & 0x700) >> 7), buf, 1, 100);
  if (r != HAL_OK)
    HAL_NVIC_SystemReset();
  r = HAL_I2C_Master_Receive(&hi2c1, EEPROM_ADDR | ((p & 0x700) >> 7), buf, 1, 100);
  if (r != HAL_OK)
    HAL_NVIC_SystemReset();
  return buf[0];
}

uint16_t eeprom_read_word(uint16_t p) {
  uint8_t buf[4];
  buf[0] = p & 0xFF;
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR | ((p & 0x700) >> 7), buf, 1, 100);
  if (r != HAL_OK)
    HAL_NVIC_SystemReset();
  r = HAL_I2C_Master_Receive(&hi2c1, EEPROM_ADDR | ((p & 0x700) >> 7), buf, 2, 100);
  if (r != HAL_OK)
    HAL_NVIC_SystemReset();
  uint16_t value = ((uint16_t)buf[0] | (((uint16_t)buf[1]) << 8));
  return value;
}

void eeprom_write_byte(uint16_t p, uint8_t val) {
  uint8_t buf[4];
  buf[0] = p & 0xFF;
  buf[1] = val;
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR | ((p & 0x700) >> 7), buf, 2, 100);
  if (r != HAL_OK)
    HAL_NVIC_SystemReset();
  HAL_Delay(5);
}

void eeprom_write_word(uint16_t p, uint16_t val) {
  uint8_t buf[4];
  buf[0] = p & 0xFF;
  buf[1] = (uint8_t)val;
  buf[2] = (uint8_t)(val >> 8);
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR | ((p & 0x700) >> 7), buf, 3, 100);
  if (r != HAL_OK)
    HAL_NVIC_SystemReset();
  HAL_Delay(5);
}