#pragma once
#include "Block.h"
#include "vector"

class World;

class Chunk : public sf::Drawable
{
public:
	Chunk(World& world, int world_x, int world_y);

    void SetBlock(int x, int y, Block block);

private:
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const override;

private:
    World& world_;
	const int world_x_;
	const int world_y_;

	std::vector<Block> blocks_;
	sf::VertexArray vertices_;
};
