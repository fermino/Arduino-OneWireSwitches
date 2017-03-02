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

	template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
	class OneWireSwitches
	{
		private:
			uint8_t InputPin = INPUT_PIN;

			uint8_t KeyArraySize = KEY_ARRAY_SIZE;
			uint16_t ExpectedValues[KEY_ARRAY_SIZE];

			uint8_t ReadTolerance;

			/*#ifdef OWS_ENABLE_CALLBACKS
				uint8_t CallbackAmount;

				OWSCallback* Callbacks;
			#endif*/
		public:
			OneWireSwitches(uint32_t* R1, uint32_t R2, uint8_t ReadTolerance = 25);

			/*#ifdef OWS_ENABLE_CALLBACKS
				void setCallbacks(uint8_t Amount, OWSCallback* Cs);

				void readAll();
			#endif*/

			bool readKey(uint8_t Key);

			uint16_t readKeyPulse(uint8_t Key, uint16_t Timeout = 0);
	};

	template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
	OneWireSwitches<KEY_ARRAY_SIZE, INPUT_PIN>::OneWireSwitches(uint32_t* R1, uint32_t R2, uint8_t ReadTolerance /* = 25 */)
	{
		this->ReadTolerance = ReadTolerance * 2;

		for(uint8_t i = 0; i < KeyArraySize; i++)
			ExpectedValues[i] = round(((float) R2 / (float) (R2 + R1[i])) * 1023) - ReadTolerance;
	}

	template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
	bool OneWireSwitches<KEY_ARRAY_SIZE, INPUT_PIN>::readKey(uint8_t KeyIndex)
	{
		if(KeyIndex < KeyArraySize)
		{
			uint16_t Reading = analogRead(InputPin);

			/*Serial.print("i = ");
			Serial.print(ExpectedValues[KeyIndex]);
			Serial.print("\t");
			Serial.print(Reading);
			Serial.print("\t");
			Serial.println(ExpectedValues[KeyIndex] + ReadTolerance);*/

			if(Reading < ExpectedValues[KeyIndex] || Reading > (ExpectedValues[KeyIndex] + ReadTolerance))
				return false;

			return true;
		}

		return false;
	}

	template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
	uint16_t OneWireSwitches<KEY_ARRAY_SIZE, INPUT_PIN>::readKeyPulse(uint8_t KeyIndex, uint16_t Timeout /* = 0 */)
	{
		uint16_t Time = 0;

		while(readKey(KeyIndex) && (Timeout == 0 || Time < Timeout))
		{
			Time++;
			delay(1);
		}

		return Time;
	}
#endif