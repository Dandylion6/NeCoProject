#pragma once
#include <cstdint>
#include <random>


namespace Nc
{

class Random final
{
public:
	// ------ Constructors ------
	explicit Random(const uint32_t seed = std::random_device{ }()) : engine(seed) { };


	// ------ Functions ------

	/**
	 * @brief Generates a random floating-point value between the given range.
	 * 
	 * @param min Minimum value (inclusive).
	 * @param max Maximum value (inclusive).
	 * @return A random float within [min, max].
	 */
	float RangeFloat(float min, float max)
	{
		if (min > max)
		{
			const float temporaryMin = min;
			max = min;
			min = temporaryMin;
		}
		std::uniform_real_distribution distribution(min, max);
		return distribution(engine);
	}


	/**
	 * @brief Generates a random integer between the given range.
	 * 
	 * @param min Minimum value (inclusive).
	 * @param max Maximum value (inclusive).
	 * @return A random int within [min, max].
	 */
	int RangeInt(int min, int max)
	{
		if (min > max)
		{
			const int temporaryMin = min;
			max = min;
			min = temporaryMin;
		}
		std::uniform_int_distribution distribution(min, max);
		return distribution(engine);
	}


	/**
	 * @brief Generates a random unsigned 8-bit integer between the given range.
	 * 
	 * @param min Minimum value (inclusive).
	 * @param max Maximum value (inclusive).
	 * @return A random uint8_t within [min, max].
	 */
	uint8_t RangeU8(uint8_t min, uint8_t max)
	{
		if (min > max)
		{
			const uint8_t temporaryMin = min;
			max = min;
			min = temporaryMin;
		}
		std::uniform_int_distribution distribution(static_cast<int>(min), static_cast<int>(max));
		return static_cast<uint8_t>(distribution(engine));
	}

private:
	// ------ Members ------
	std::mt19937 engine;

};

}
