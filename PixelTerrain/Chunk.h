#pragma once
#include <string>
#include <vector>
#include "Block.h"

class World;

class Chunk : public sf::Drawable
{
public:
	Chunk(World& world, int world_x, int world_y);

	Block GetBlock(int x, int y) const;
    void SetBlock(int x, int y, Block block);

private:
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const override;

private:
    World& world_;
	const int world_x_;
	const int world_y_;

	std::vector<Block> blocks_;
	sf::VertexArray vertices_;

	/*sf::Font font_;
	sf::Text text_;*/
};
