#include<Wire.h>

// I2C address of the MPU-6050.
const int mpuAddress = 0x68;

int16_t accelX;
int16_t accelY;
int16_t accelZ;
int16_t temperature;
int16_t gyroscopeX;
int16_t gyroscopeY;
int16_t gyroscopeZ;

char line[132];


void setup()
{
	Wire.begin();
	Wire.beginTransmission(mpuAddress);

	// PWR_MGMT_1 register.
	Wire.write(0x6B);

	// set to zero (wakes up the MPU-6050).
	Wire.write(0);

	Wire.endTransmission(true);

	Serial.begin(115200);

	// Waiting for Serial Monitor.
	while (!Serial);

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
	Wire.beginTransmission(mpuAddress);
	
	// Starting with register 0x3B (ACCEL_XOUT_H).
	Wire.write(0x3B);

	Wire.endTransmission(false);

	// Request a total of 14 registers.
	Wire.requestFrom(mpuAddress, 14, true);

	// 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
	accelX = Wire.read() << 8 | Wire.read();

	// 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
	accelY = Wire.read() << 8 | Wire.read();

	// 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
	accelZ = Wire.read() << 8 | Wire.read();

	// 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
	temperature = Wire.read() << 8 | Wire.read();

	// 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
	gyroscopeX = Wire.read() << 8 | Wire.read();

	// 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
	gyroscopeY = Wire.read() << 8 | Wire.read();

	// 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
	gyroscopeZ = Wire.read() << 8 | Wire.read();

	// Position the cursor.
	sprintf(line, "%c[2;1H", 0x1b);
	Serial.print(line);

	// Clear the line.
	sprintf(line, "%c[2K", 0x1b);
	Serial.print(line);

	// Colour change.
	sprintf(line, "%c[36m", 0x1b);
	Serial.print(line);

	// Note: I had issues with the value outputs when I used an sprintf for the output, thus this mess.
	Serial.print(accelX); Serial.print("\t");
	Serial.print(accelY); Serial.print("\t");
	Serial.print(accelZ); Serial.print("\t");

	// Colour change.
	sprintf(line, "%c[31m", 0x1b);
	Serial.print(line);

	// Equation for temperature in degrees C from datasheet.
	Serial.print(temperature / 340.00 + 36.53); Serial.print("\t");

	// Colour change.
	sprintf(line, "%c[34m", 0x1b);
	Serial.print(line);

	Serial.print(gyroscopeX); Serial.print("\t");
	Serial.print(gyroscopeY); Serial.print("\t");
	Serial.print(gyroscopeZ); Serial.print("\t");
	
	Serial.println();

	delay(250);
}
