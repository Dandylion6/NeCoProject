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
	float RangeFloat(const float min, const float max)
	{
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
	int RangeInt(const int min, const int max)
	{
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
	uint8_t RangeU8(const uint8_t min, const uint8_t max)
	{
		std::uniform_int_distribution<int16_t> distribution(min, max);
		return static_cast<uint8_t>(distribution(engine));
	}

private:
	// ------ Members ------
	std::mt19937 engine;

};

}
