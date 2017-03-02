	#include <OneWireSwitches.h>

	#define INPUT_PIN A0
	#define KEY_AMOUNT 2

	#define R2 20000 // 20k

	uint32_t R1Values[KEY_AMOUNT]
	{
		10000,	// 10k
		5000	// 5k
	};

	#define TOLERANCE 25

	OneWireSwitches<KEY_AMOUNT, INPUT_PIN> Switches(R1Values, R2, TOLERANCE);

	void setup()
	{
		Serial.begin(9600);
	}

	void loop()
	{
		if(Switches.readKey(0))
		{
			Serial.println("Button 0 pressed");
		}

		if(Switches.readKey(1))
		{
			Serial.println("Button 1 pressed");
		}
	}