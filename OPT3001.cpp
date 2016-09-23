#include "OPT3001.h"

uint16_t deviceID = 0
uint16_t sensor_data = 0;
float sensor_data1 = 0.0;
char temp_write[3] = {0,0,0};
float lux_multiplier = 10.24; //Range set to RN[3:0] = 1010 in config register

I2C i2c(PB_9, PB_8);

void read_sensor()
{
    printf("Inside read sensor..");
    i2c.frequency(400000);
    //Configure light sensor
    temp_write[0]=0x01;
    temp_write[1]=0xAE; //C810h is the default content for configuration register (address=01h) (Current cofig = AE0C)
    temp_write[2]=0x0C;
    
    int a = i2c.write(address, temp_write, 3, 0);
    //printf("a ====== %d",a);

    //Read Device ID
    temp_write[0] = 0x7F; //device ID register address
    char data[2] = {0,0};
    i2c.write(address, temp_write, 1, false);
    int b = i2c.read(address, data, 2, 0);
    //printf("b ======= =  == == %d", b);
    deviceID = (((uint16_t)data[0]) << 8) | data[1];
    //printf("Device ID= %d\n", deviceID);
    if(deviceID == 0x3001) { //12289d (3001h) = device ID from datasheet
        printf("Device ID OK.....");        
    }
    
    //From datasheet: Read result register (mask bits 15:13 - AND with suitable mask) & Multiply contents by lux_multiply factor to get reading.
    while(1) {
        temp_write[0] = 0x00; //result register address
        char opt_data[2] = {0,0};
        i2c.write(address, temp_write, 1, false);
        int c = i2c.read(address, opt_data, 2, 0);
        //printf("opt_data[1] ==== %d\t", opt_data[1]);
        //printf("opt_data[0] ==== %d\t", opt_data[0]);
        sensor_data1 = ((((uint16_t)opt_data[0]) << 8) | opt_data[1]) & 0x1FFF;
        //printf("Sensor data = %0.2f\n", sensor_data1);
        sensor_data1 = sensor_data1 * lux_multiplier;
        printf("Lux data = %f\n", sensor_data1);
/*
        if(sensor_data1 == 0) {
            led1 = 1;
            lcd.DisplayStringAt(3, LINE(4), (uint8_t *)"Too Dark..Turning ON LED", CENTER_MODE);
        } else {
            led1 = 0;
            lcd.DisplayStringAt(3, LINE(4), (uint8_t *)"All OK. No lights needed", CENTER_MODE);
        }
        Thread::wait(1000);
*/
    }
}
