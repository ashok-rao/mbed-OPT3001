#include "OPT3001.h"

uint16_t deviceID = 0;
uint16_t sensor_data = 0;
float sensor_data1 = 0.0;
char temp_write[3] = {0,0,0};
float lux_multiplier = 10.24; //Range set to RN[3:0] = 1010 in config register. Refer datasheet for lux_multiplier options.

//I2C peripheral pins on the STM32F7-DISCOVERY & NUCLEO_F429ZI board.
I2C i2c(PB_9, PB_8);

  //Read Device ID
void read_DeviceID() {
    temp_write[0] = 0x7F; //device ID register address from datasheet
    char data[2] = {0,0};
    i2c.write(address, temp_write, 1, false);
    int b = i2c.read(address, data, 2, 0);
    //printf("b ======= =  == == %d", b);
    deviceID = (((uint16_t)data[0]) << 8) | data[1];
    //printf("Device ID= %d\n", deviceID);
    if(deviceID == 0x3001) { //12289d (3001h) = device ID from datasheet. Factory programmed.
        printf("I2C Device ID OK.....");      
   }
}
    

float read_sensor() {
    //printf("Inside read sensor..");
    i2c.frequency(400000);
    //Configure light sensor
    temp_write[0]=0x01;
    temp_write[1]=0xAE; //C810h is the default content for configuration register (address=01h) (Current cofig = AE0C)
    temp_write[2]=0x0C;
    
    int a = i2c.write(address, temp_write, 3, 0);
    //printf("a ====== %d",a);

    //From datasheet: Read result register (mask bits 15:13 - AND with suitable mask) & Multiply contents by 
    //lux_multiply factor to get reading. This configuration of the sensor is achieved because of the exponent in the configuration
    //register being set to 0's.
//    while(1) {
        temp_write[0] = 0x00; //result register address
        char opt_data[2] = {0,0};
        i2c.write(address, temp_write, 1, false);
        int c = i2c.read(address, opt_data, 2, 0);
        //printf("opt_data[1] ==== %d\t", opt_data[1]);
        //printf("opt_data[0] ==== %d\t", opt_data[0]);
        sensor_data1 = ((((uint16_t)opt_data[0]) << 8) | opt_data[1]) & 0x1FFF;
        //printf("Sensor data = %0.2f\n", sensor_data1);
        sensor_data1 = sensor_data1 * lux_multiplier;
     //   printf("Lux data = %f\n", sensor_data1);
//    }
    return sensor_data1;
}
