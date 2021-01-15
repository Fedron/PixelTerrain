#pragma once
#include "ColorScale.h"

class DayNight
{
public:
	explicit DayNight(int gradient_detail, sf::Vector2i world_size, int block_size);
	~DayNight() = default;

	/**
	 * Updates the day/night cycle
	 *
	 * @param delta_time Reference to the delta time
	 */
	void Update(float& delta_time);

	/**
	 * Draws the sky to the window
	 *
	 * @param window Reference to the window to which to draw to
	 */
	void Draw(sf::RenderWindow& window);

private:
	ColorScale gradient_;
	int gradient_detail_;
	sf::Color* tab_;

	int dir_ = 1;
	float day_progress_;

	sf::VertexArray sky_;

	sf::Texture sun_texture_;
	sf::Sprite sun_;
};
