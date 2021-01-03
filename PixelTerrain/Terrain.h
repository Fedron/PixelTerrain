#pragma once
#include <noise/noise.h>
#include <SFML/Graphics.hpp>

class Terrain
{
public:
	Terrain(int width, int height, int min_surface_level, int max_surface_level);
	~Terrain();

	void Generate();
	const sf::Sprite* GetSprite() const;

private:
	const int width_;
	const int height_;
	const int min_surface_level_;
	const int max_surface_level_;
	
	sf::Uint8* texture_pixels_;
	
	sf::Texture texture_;
	sf::Sprite sprite_;

	noise::module::Perlin perlin_noise_;
};
