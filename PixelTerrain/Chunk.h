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

class World;

// TODO: Inherit sf::Drawable and sf::Transformable
// TODO: Try using vertex arrays, could be faster (also allows for texturing)
class Chunk
{
public:
	Chunk(World* world, int world_x, int world_y);
	~Chunk() = default;
	
	Block GetBlock(int x, int y) const;
	void SetBlock(int x, int y, Block block);

	void Update();
	void Draw(sf::RenderWindow* window);

private:
	World* world_;
	const int world_x_;
	const int world_y_;

	std::vector<Block> blocks_;
	sf::Uint8* texture_pixels_;
	bool is_dirty_;

	sf::Texture texture_;
	sf::Sprite sprite_;
};
