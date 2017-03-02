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
		uint16_t PulseLength = Switches.readKeyPulse(0);

		if(PulseLength > 0)
		{
			Serial.print("Button 0 pressed ");
			Serial.print(PulseLength);
			Serial.println("ms");
		}

		// This detects a keypress larger than 25ms with a timeout of 50
		if(Switches.readKeyPulse(1, 50) == 25)
		{
			Serial.println("Button 1 pressed"); 
		}
	}