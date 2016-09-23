#include "mbed.h"

//I2C address of OPT3001 on the BOOSTXL-SENSORS breakout board from TI.
//Please change this according to your board / I2C address allocated or required.
//mbed I2C API use 7 bit address, hence left shift the 8-bit address by 1 bit.
const int address = 0x47 << 1;

void read_sensor();
