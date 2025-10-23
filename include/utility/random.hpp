#pragma once
#include <cstdint>
#include <random>


namespace Nc
{
	class Random
	{
	public:
		static float Range(float min, float max)
		{
			std::random_device device;
			std::mt19937 generator(device());

			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(generator);
		}


		static int32_t Range(int32_t min, int32_t max)
		{
			std::random_device device;
			std::mt19937 generator(device());

			std::uniform_int_distribution<int32_t> distribution(min, max);
			return distribution(generator);
		}

		
		static uint8_t RangeU8(uint8_t min, uint8_t max)
		{
			std::random_device device;
			std::mt19937 generator(device());

			std::uniform_int_distribution<int16_t> distribution(min, max);
			return static_cast<uint8_t>(distribution(generator));
		}
	};
}