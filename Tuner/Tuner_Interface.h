#pragma once
#include <cstdbool>
#include <cstddef>
#include <cstdint>

bool Tuner_WriteBuffer(uint8_t *buf, uint16_t len);
bool Tuner_ReadBuffer(uint8_t *buf, uint16_t len);

bool Tuner_Patch(const uint8_t TEF);
bool Tuner_Init(void);
bool Tuner_Init4000(void);
bool Tuner_Init9216(void);
bool Tuner_Init12000(void);