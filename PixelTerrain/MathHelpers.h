#pragma once
#pragma warning(disable: 4244 26451)

namespace maths
{
	/**
	 * Remaps a value from one range to another
	 *
	 * @param value The value to remap
	 * @param min The original minimum value
	 * @param max The original maximum value
	 * @param new_min The new minimum value
	 * @param new_max The new maximum value
	 *
	 * @returns The original value remapped to the new range
	 */
	inline int Remap(const int value, const int min, const int max, const int new_min, const int new_max)
	{
		return new_min + (value - min) * (new_max - new_min) / (max - min);
	}

	/**
	 * Remaps a value from one range to another
	 *
	 * @param value The value to remap
	 * @param min The original minimum value
	 * @param max The original maximum value
	 * @param new_min The new minimum value
	 * @param new_max The new maximum value
	 *
	 * @returns The original value remapped to the new range
	 */
	inline int Remap(const double value, const int min, const int max, const int new_min, const int new_max)
	{
		return new_min + (value - min) * (new_max - new_min) / (max - min);
	}

	/**
	 * Gets the sign of a given value
	 *
	 * @param value The value to get the sign for
	 * @returns -1 for a negative value, 1 for a positive value, and 0 for everything else
	 */
	inline int Sign(const int value)
	{
		if (value > 0) return 1;
		if (value < 0) return -1;
		return 0;
	}
}