#ifndef OneWireSwitches_h
#define OneWireSwitches_h
 
	#if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif

	//#define OWS_ENABLE_CALLBACKS

	/*#ifdef OWS_ENABLE_CALLBACKS
		typedef void (*ows_callback)();

		struct OWSCallback
		{
			uint8_t R1Index;
			uint16_t MinPulseLength;
			uint16_t MaxPulseLength;
			ows_callback Callback;
		};
	#endif*/

	template <uint8_t KEY_ARRAY_SIZE>
	class OneWireSwitches
	{
		private:
			uint8_t InputPin;

			uint16_t ExpectedValues[KEY_ARRAY_SIZE];
			uint8_t ReadTolerance;

			/*#ifdef OWS_ENABLE_CALLBACKS
				uint8_t CallbackAmount;

				OWSCallback* Callbacks;
			#endif*/
		public:
			OneWireSwitches(uint8_t InputPin, uint32_t* R1, uint32_t R2, uint8_t ReadTolerance = 25);

			/*#ifdef OWS_ENABLE_CALLBACKS
				void setCallbacks(uint8_t Amount, OWSCallback* Cs);

				void readAll();
			#endif*/

			bool readKey(uint8_t Key);

			//uint16_t readPulse(uint8_t Key, uint16_t Timeout /* = 0*/);
	};

	template <uint8_t KEY_ARRAY_SIZE>
	OneWireSwitches<KEY_ARRAY_SIZE>::OneWireSwitches(uint8_t InputPin, uint32_t* R1, uint32_t R2, uint8_t ReadTolerance = 25)
	{
		this->InputPin = InputPin;
		this->ReadTolerance = ReadTolerance * 2;

		for(uint8_t i = (sizeof(ExpectedValues) / sizeof(*ExpectedValues)) - 1; i >= 0; i--)
			ExpectedValues[i] = round(R2 / (R2 + R1[i]) * 1023) - ReadTolerance;
	}

	template <uint8_t KEY_ARRAY_SIZE>
	bool OneWireSwitches<KEY_ARRAY_SIZE>::readKey(uint8_t KeyIndex)
	{
		if(KeyIndex < (sizeof(ExpectedValues) / sizeof(*ExpectedValues)))
		{
			uint16_t Reading = analogRead(InputPin);

			if(Reading < ExpectedValues[KeyIndex] || Reading > (ExpectedValues[KeyIndex] + ReadTolerance))
				return false;

			return true;
		}

		return false;
	}
#endif