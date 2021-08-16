#include<Wire.h>

// I2C address of the MPU-6050.
const int MPU_addr=0x68;

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
char line[132];



void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  
  // PWR_MGMT_1 register.
  Wire.write(0x6B);
  
  // set to zero (wakes up the MPU-6050).
  Wire.write(0);
  
  Wire.endTransmission(true);
  Serial.begin(57600);

  // Position the cursor.
  sprintf(line, "%c[1;1H", 0x1b);
  Serial.print(line);
  
  // Clear the line.
  sprintf(line, "%c[2K", 0x1b);
  Serial.print(line);

  Serial.println("AcX     AcY     AcZ     Temp    GyX     GyY     GyZ");
}


void loop()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  
  // Request a total of 14 registers.
  Wire.requestFrom(MPU_addr, 14, true);
  
  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcX=Wire.read() << 8 | Wire.read();
  
  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcY=Wire.read() << 8 | Wire.read();
  
  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  AcZ=Wire.read() << 8 | Wire.read();
  
  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  Tmp=Wire.read() << 8 | Wire.read();
  
  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyX=Wire.read() << 8 | Wire.read();
  
  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyY=Wire.read() << 8 | Wire.read();
  
  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  GyZ=Wire.read() << 8 | Wire.read();
  
  // Position the cursor.
  sprintf(line, "%c[2;1H", 0x1b);
  Serial.print(line);
  
  // Clear the line.
  sprintf(line, "%c[2K", 0x1b);
  Serial.print(line);

  sprintf(line, "%c[36m", 0x1b);
  Serial.print(line);

  // Note: I had issues with the value outputs when I used an sprintf for the output, thus this mess.
  Serial.print(AcX);Serial.print("\t");
  Serial.print(AcY);Serial.print("\t");
  Serial.print(AcZ);Serial.print("\t");
  
  sprintf(line, "%c[31m", 0x1b);
  Serial.print(line);

  // Equation for temperature in degrees C from datasheet.
  Serial.print(Tmp / 340.00 + 36.53);Serial.print("\t");

  sprintf(line, "%c[34m", 0x1b);
  Serial.print(line);

  Serial.print(GyX);Serial.print("\t");
  Serial.print(GyY);Serial.print("\t");
  Serial.print(GyZ);Serial.print("\t");
  Serial.println();

  delay(5);
}
