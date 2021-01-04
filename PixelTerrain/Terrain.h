#pragma once
#include <utility>
#include <vector>
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

	void AddGenerationStep(void (*step)(Terrain* terrain));
	void Generate();
	
	double GetNoise(int x, int y) const;

	void SetBlock(int x, int y, Block block);
	Block GetBlock(int x, int y) const;

	void SetBlocks(std::vector<Block> blocks);
	std::vector<Block> GetBlocks() const
	{
		return blocks_;
	}
	
	void Draw(sf::RenderWindow* window);

public:
	const int width_;
	const int height_;
	int min_surface_level_;
	int max_surface_level_;
	const int grass_layer_height_; 
	const int dirt_layer_height_;

	noise::module::Perlin perlin_noise_;

private:
	std::vector<Block> blocks_;
	sf::Uint8* texture_pixels_;
	bool is_dirty_;
	
	sf::Texture texture_;
	sf::Sprite sprite_;

	

	std::vector<void (*)(Terrain* terrain)> generation_steps_;
};
