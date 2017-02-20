#ifndef OneWireSwitches_h
#define OneWireSwitches_h
 
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif

	//#define OWS_ENABLE_CALLBACKS

	#ifdef OWS_ENABLE_CALLBACKS
		typedef void (*ows_callback)();

		struct OWSCallback
		{
			uint8_t R1Index;
			uint16_t MinPulseLength;
			uint16_t MaxPulseLength;
			ows_callback Callback;
		};
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

			#ifdef OWS_ENABLE_CALLBACKS
				uint8_t CallbackAmount;

				OWSCallback* Callbacks;
			#endif 
		public:
			OneWireSwitches(uint8_t InputPin, uint8_t KeyAmount, uint32_t* R1Array, uint32_t R2, uint8_t Tolerance = 50)
			: InputPin(InputPin), KeyAmount(KeyAmount), R1Array(R1Array), R2(R2), Tolerance(Tolerance)
			{}

			#ifdef OWS_ENABLE_CALLBACKS
				void setCallbacks(uint8_t Amount, OWSCallback* Cs);

				void readAll();
			#endif

			bool readKey(uint8_t Key);

			uint16_t readPulse(uint8_t Key, uint16_t Timeout /* = 0*/);
	};
#endif