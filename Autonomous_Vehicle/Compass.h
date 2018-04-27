#include <Wire.h>
//#include <MPU9250_asukiaaa.h>

// Magnetometer
//MPU9250 mag;



// minimum and maximum values seen during calibration time
int16_t mag_low_x = 2411;
int16_t mag_high_x = 3254;
int16_t mag_low_y = -3201;
int16_t mag_high_y = -2227;
int16_t mag_low_z = -581;
int16_t mag_high_z = 189;
#define degrees_per_radian  (180./3.14159265358979)
#define MAG_ADDR 0x0E //7-bit address for the MAG3110, doesn't change



// Used for testing to check accuracy of readings
void print_values(int16_t x, int16_t y, int16_t z)
{
  Serial.print("x=");
  Serial.print(x);
  Serial.print(",");
  Serial.print("y=");
  Serial.print(y);
  Serial.print(",");
  Serial.print("z=");
  Serial.println(x);
}

// Used to print Heading to Serial Port and also to the OLED devices
void print_heading(int16_t x, int16_t y, int16_t z)
{
  float mag_x_scale = 1.0 / (mag_high_x - mag_low_x);
  float mag_y_scale = 1.0 / (mag_high_y - mag_low_y);


  float heading = atan2(-y * mag_y_scale, x * mag_x_scale);
  if (heading < 0)
  {
    heading += 2 * PI; // correct for when the heading is negative
  }
  float headingDegrees = heading * degrees_per_radian;  // convert to degrees

  Serial.print("headingDegrees = " );
  Serial.print(headingDegrees);
  Serial.println();
}

// Read X Axis
int readx(void)
{
  int xl, xh; //define the MSB and LSB

  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x01); // x MSB reg
  Wire.endTransmission(); // stop transmitting
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while (Wire.available()) // slave may send less than requested
  {
    xh = Wire.read(); // receive the byte
  }
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x02); // x LSB reg
  Wire.endTransmission(); // stop transmitting
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while (Wire.available()) // slave may send less than requested
  {
    xl = Wire.read(); // receive the byte
  }
  int xout = (xl | (xh << 8)); //concatenate the MSB and LSB
  return xout;
}

//Set the Compass is ready mode
int ready(void)
{
  int yl, yh; //define the MSB and LSB
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x03); // y MSB reg
  Wire.endTransmission(); // stop transmitting
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while (Wire.available()) // slave may send less than requested
  {
    yh = Wire.read(); // receive the byte
  }
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x04); // y LSB reg
  Wire.endTransmission(); // stop transmitting
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while (Wire.available()) // slave may send less than requested
  {
    yl = Wire.read(); // receive the byte
  }
  int yout = (yl | (yh << 8)); //concatenate the MSB and LSB
  return yout;
}

// Read Z Axis
int readz(void)
{
  int zl, zh; //define the MSB and LSB

  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x05); // z MSB reg
  Wire.endTransmission(); // stop transmitting

  delayMicroseconds(2); //needs at least 1.3us free time between start and stop

  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while (Wire.available()) // slave may send less than requested
  {
    zh = Wire.read(); // receive the byte
  }

  delayMicroseconds(2); //needs at least 1.3us free time between start and stop

  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x06); // z LSB reg
  Wire.endTransmission(); // stop transmitting
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while (Wire.available()) // slave may send less than requested
  {
    zl = Wire.read(); // receive the byte
  }
  int zout = (zl | (zh << 8)); //concatenate the MSB and LSB
  return zout;
}

//Update Compass Readings
void getCompassReadings(void) {
  int mag_x, mag_y, mag_z;
  mag_x = readx();
  mag_y = ready();
  mag_z = readz();
  //print_values();
//  print_values(mag_x, mag_y, mag_z);
  print_heading(mag_x, mag_y, mag_z);
  delay(5);
}

//Set Up Compass 
void configCompass(void)
{
Wire.begin(); // join i2c bus (address optional for master)  
Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
Wire.write(0x11); // cntrl register2
Wire.write(0x80); // send 0x80, enable auto resets
Wire.endTransmission(); // stop transmitting
delay(15);
Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
Wire.write(0x10); // cntrl register1
Wire.write(1); // send 0x01, active mode
Wire.endTransmission(); // stop transmitting
}


