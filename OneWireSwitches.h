#ifndef OneWireSwitches_h
#define OneWireSwitches_h
 
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
		#include "pins_arduino.h"
		#include "WConstants.h"
	#endif

	class OneWireSwitches
	{
		private:
			uint8_t InputPin;

			uint8_t KeyAmount;

			uint32_t* R1Array;
			uint32_t R2; // Resistor tied to ground

			// Resistor's % tolerance multiplied by 10
			// Then, a 5% tolerance will be 50, and a 1% tolerance will be 10. 
			uint8_t Tolerance;

		public:
			OneWireSwitches(uint8_t InputPin, uint8_t KeyAmount, uint32_t* R1Array, uint32_t R2, uint8_t Tolerance = 50)
			: InputPin(InputPin), KeyAmount(KeyAmount), R1Array(R1Array), R2(R2), Tolerance(Tolerance)
			{}

			bool readKey(uint8_t Key);
	};
#endif