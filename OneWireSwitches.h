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
			uint8_t KeyIndex;
			uint16_t MinPulseLength;
			uint16_t MaxPulseLength;
			ows_callback Callback;
		};
	#endif

	template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
	class OneWireSwitches
	{
		private:
			uint8_t InputPin = INPUT_PIN;

			uint8_t KeyArraySize = KEY_ARRAY_SIZE;
			uint16_t ExpectedValues[KEY_ARRAY_SIZE];

			uint8_t ReadTolerance;

			#ifdef OWS_ENABLE_CALLBACKS
				uint8_t CallbackArraySize = 0;
				OWSCallback* Callbacks;

				int16_t LastReading = -1;
			#endif
		public:
			OneWireSwitches(uint32_t* R1, uint32_t R2, uint8_t ReadTolerance = 25);

			#ifdef OWS_ENABLE_CALLBACKS
				void setCallbacks(uint8_t Amount, OWSCallback* Cs);
				void readKeys();
			#endif

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

	#ifdef OWS_ENABLE_CALLBACKS
		template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
		void OneWireSwitches<KEY_ARRAY_SIZE, INPUT_PIN>::setCallbacks(uint8_t CS, OWSCallback* C)
		{
			CallbackArraySize = CS;
			Callbacks = C;
		}

		template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
		void OneWireSwitches<KEY_ARRAY_SIZE, INPUT_PIN>::readKeys()
		{
			// Use the same reading for all buttons
			LastReading = analogRead(InputPin);

			for(uint8_t i = 0; i < CallbackArraySize; i++)
			{
				if(Callbacks[i].Callback != NULL)
				{
					uint16_t PulseLength = readKeyPulse(Callbacks[i].KeyIndex, Callbacks[i].MaxPulseLength);

					if(PulseLength >= Callbacks[i].MinPulseLength)
						Callbacks[i].Callback();
				}
			}

			// Clear the flag
			LastReading = -1;
		}
	#endif

	template <uint8_t KEY_ARRAY_SIZE, uint8_t INPUT_PIN>
	bool OneWireSwitches<KEY_ARRAY_SIZE, INPUT_PIN>::readKey(uint8_t KeyIndex)
	{
		if(KeyIndex < KeyArraySize)
		{
			#ifdef OWS_ENABLE_CALLBACKS
				uint16_t Reading = LastReading != -1 ? LastReading : analogRead(InputPin);
			#else
				uint16_t Reading = analogRead(InputPin);
			#endif

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

		while(readKey(KeyIndex) && (Time < Timeout || Timeout == 0))
		{
			Time++;
			delay(1);
		}

		return Time;
	}
#endif