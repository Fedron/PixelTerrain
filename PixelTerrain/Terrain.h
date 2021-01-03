#pragma once
#include <chrono>
#include <iostream>
#include <vector>
#include <noise/noise.h>
#include <SFML/Graphics.hpp>
#include "Block.h"
#include "MathHelpers.h"

class Terrain
{
public:
	Terrain(
		int width, int height, int min_surface_level, int max_surface_level,
		int grass_layer_height, int dirt_layer_height);
	~Terrain();

	void AddGenerationStep(void (*step)(const Terrain* terrain));
	void Generate();
	
	const sf::Sprite* GetSprite() const;
	double GetNoise(int x, int y) const;

	void SetBlock(int x, int y, Block block) const;
	void SetDirty();
	void UpdateTexture();

public:
	const int width_;
	const int height_;
	const int min_surface_level_;
	const int max_surface_level_;
	const int grass_layer_height_;
	const int dirt_layer_height_;

private:
	sf::Uint8* texture_pixels_;
	bool is_dirty_;
	
	sf::Texture texture_;
	sf::Sprite sprite_;

	noise::module::Perlin perlin_noise_;

	std::vector<void (*)(const Terrain* terrain)> generation_steps_;
};
