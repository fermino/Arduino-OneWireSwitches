#include "OneWireSwitches.h"

	// Resistor's tolerance in percent divided by 100. The user previously multiplied it by 10
	#define OWS_RESISTOR_TOLERANCE Tolerance / 1000.0f

	// Calculate lower and higher ranges based on resistor tolerances

	#define OWS_LOWER_R1	R1 * (1.0f - OWS_RESISTOR_TOLERANCE)
	#define OWS_LOWER_R2	R2 * (1.0f - OWS_RESISTOR_TOLERANCE)

	#define OWS_HIGHER_R1	R1 * (1.0f + OWS_RESISTOR_TOLERANCE)
	#define OWS_HIGHER_R2	R2 * (1.0f + OWS_RESISTOR_TOLERANCE)

	bool OneWireSwitches::readKey(uint8_t Key)
	{
		if(Key < KeyAmount)
		{
			uint32_t R1 = R1Array[Key];

			uint16_t Reading = analogRead(InputPin);
			// If the reading is between the lower expected reading and the higher one

			if(Reading >= (1023 * OWS_LOWER_R2 / (OWS_HIGHER_R1 + OWS_LOWER_R2)) && Reading <= (1023 * OWS_HIGHER_R2 / (OWS_LOWER_R1 + OWS_HIGHER_R2)))
				return true;
		}

		return false;
	}