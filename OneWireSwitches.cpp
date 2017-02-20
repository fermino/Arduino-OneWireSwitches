#include "OneWireSwitches.h"

	#ifdef OWS_ENABLE_CALLBACKS
		void OneWireSwitches::setCallbacks(uint8_t Amount, OWSCallback* Cs)
		{
			CallbackAmount = Amount;
			Callbacks = Cs;
		}

		void OneWireSwitches::readAll()
		{
			// Use readPulse()
			for(uint8_t i = 0; i < CallbackAmount; i++)
			{
				uint16_t PulseLength;

				while(readKey(Callbacks[i].R1Index) && PulseLength < Callbacks[i].MaxPulseLength)
				{
					PulseLength++;
					delay(1);
				}

				if(PulseLength >= Callbacks[i].MinPulseLength)
					Callbacks[i].Callback();
			}
		}
	#endif
	
	// Resistor's tolerance in percent divided by 100. The user previously multiplied it by 10
	#define OWS_RESISTOR_TOLERANCE Tolerance / 1000.0f

	// Calculate lower and higher ranges based on resistor tolerances

	#define OWS_R1 R1Array[Key]

	#define OWS_LOWEST_R1	OWS_R1 * (1.0f - OWS_RESISTOR_TOLERANCE)
	#define OWS_LOWEST_R2	R2 * (1.0f - OWS_RESISTOR_TOLERANCE)

	#define OWS_HIGHEST_R1	OWS_R1 * (1.0f + OWS_RESISTOR_TOLERANCE)
	#define OWS_HIGHEST_R2	R2 * (1.0f + OWS_RESISTOR_TOLERANCE)

	bool OneWireSwitches::readKey(uint8_t Key)
	{
		if(Key < KeyAmount)
		{
			uint16_t Reading = analogRead(InputPin);
			// If the reading is between the lower expected reading and the higher one

			if(Reading >= (1023 * OWS_LOWEST_R2 / (OWS_HIGHEST_R1 + OWS_LOWEST_R2)) && Reading <= (1023 * OWS_HIGHEST_R2 / (OWS_LOWEST_R1 + OWS_HIGHEST_R2)))
				return true;
		}

		return false;
	}

	// Reads a pulse from a button and returns the time since the press
	// Returns 0 if the button is not pressed
	uint16_t OneWireSwitches::readPulse(uint8_t Key, uint16_t Timeout = 0)
	{
		// Use timeout instead of
		uint16_t Time = 0;

		while(readKey(Key) == HIGH && (Timeout == 0 || Time < Timeout))
		{
			Time++;
			delay(1);
		}

		return Time;
	}

	// Add copyright and license info. 
	// Add examples and keywords.txt