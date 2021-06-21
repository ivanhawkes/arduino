void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
}


void dot()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
	delay(350);	
}


void dash()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(300);
	digitalWrite(LED_BUILTIN, LOW);
	delay(200);	
}


void loop()
{
	dot();
	dot();
	dot();
	dash();
	dash();
	dash();
	dot();
	dot();
	dot();
	delay(2000);
}
