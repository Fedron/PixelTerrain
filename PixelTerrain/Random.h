#pragma once
#include <functional>
#include <random>
#include "noise/noise.h"

class Random
{
public:
	Random() = default;
	/**
	 * Creates a Random object with all the generators seeds set to the same value
	 *
	 * @param seed Seed to use for generators
	 */
	explicit Random(int seed);

	/**
	 * Re-seeds all generators
	 *
	 * @param seed The new seed
	 */
	void SetSeed(int seed);
	
	/**
	 * Sets the perlin noise frequency
	 *
	 * @param freq The new frequency
	 */
	void SetPerlinFrequency(double freq);
	
	/**
	 * Returns a perlin noise value
	 *
	 * @param x x-coordinate
	 * @param y y-coordinate
	 *
	 * @returns Perlin noise value at (x, y)
	 */
	double GetPerlin(int x, int y) const;

	/**
	 * Returns a random number in the range
	 *
	 * @param min Minimum number (inclusive)
	 * @param max Maximum number (inclusive)
	 *
	 * @returns Random number between min and max
	 */
	int GetNumber(int min, int max);

	/**
	 * Returns true or false
	 *
	 * @returns True or false
	 */
	bool GetBool() const;

private:
	// PRNG
	std::mt19937 mt_generator_{ std::random_device{}() };
	// libnoise Perlin noise module
	noise::module::Perlin perlin_;
};
