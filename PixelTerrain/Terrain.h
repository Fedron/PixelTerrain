#pragma once
#include <vector>
#include <noise/noise.h>
#include <SFML/Graphics.hpp>

class Terrain
{
public:
	Terrain(int width, int height, int min_surface_level, int max_surface_level);
	~Terrain();

	void AddGenerationStep(void (*step)(const Terrain* terrain));
	void Generate();
	
	const sf::Sprite* GetSprite() const;
	double GetNoise(int x) const;

	void SetPixel(int x, int y, sf::Color color) const;

public:
	const int width_;
	const int height_;
	const int min_surface_level_;
	const int max_surface_level_;

private:
	sf::Uint8* texture_pixels_;
	
	sf::Texture texture_;
	sf::Sprite sprite_;

	noise::module::Perlin perlin_noise_;

	std::vector<void (*)(const Terrain* terrain)> generation_steps_;
};
