#include <Wire.h>

const uint8_t I2C_ADDRESS = 0x42;


void setup()
{
	Wire.begin(I2C_ADDRESS);
	Wire.onRequest(requestEvent);
	Wire.onReceive(receiveEvent);
}


void loop()
{
	Serial.begin(115200);
	Serial.print(".");
	delay(1000);
}


void receiveEvent(int bytes)
{
	byte opcode;

	Serial.println("receive");
	opcode = Wire.read();

	// if (bytes > 1) {
	// 	speed = Wire.read();

	// 	if (opcode == COMMAND_ON_OFF) {
	// 		speed = (speed == 0x01) ? DEFAULT_SPEED : 0;
	// 	}
	// }
}


void requestEvent()
{
	Serial.println("request");
	// switch (opcode)
	// {
	// 	case COMMAND_SPEED:
	// 		Wire.write((uint8_t*)&speed, sizeof(speed));
	// 		break;

	// 	default:
	// 		Wire.write(0);
	// 		break;
	// }
}
