#pragma once
#include <string>
#include <vector>
#include "Block.h"
#include "MathHelpers.h"

class World;

class Chunk : public sf::Drawable
{
#pragma region Methods
public:
	/**
	 * Small individual container of blocks in the world
	 *
	 * @param world Reference to which to bind to
	 * @param world_x The x coordinate of the chunk in the world
	 * @param world_y The y coordinate of the chunk in the world
	 */
	Chunk(World& world, int world_x, int world_y);

	/**
	 * Gets the block at the given coordinates
	 *
	 * @param x The x coordinate, within the chunk, of the block
	 * @param y The y coordinate, within the chunk, of the block
	 *
	 * @returns The block at the coordinates
	 */
	Block* GetBlock(int x, int y) const;

	/**
	 * Sets the block at the given coordinates
	 *
	 * @param x The x coordinate, within the chunk, of the block
	 * @param y The y coordinate, within the chunk, of the block
	 * @param block The block to set to
	 */
    void SetBlock(int x, int y, Block* block);

	/** 
	 * Updates all the blocks within the chunk
	 */
	void Update();

private:	
	/**
	 * SFML draw function, draws to the window
	 */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
#pragma endregion 

#pragma region Variables
private:
	// Reference to the world in which this chunk is in
    World& world_;
	// X coordinate of the chunk in the world
	const int world_x_;
	// Y coordinate of the chunk in the world
	const int world_y_;

	// A list of all the blocks in the chunk (row-major)
	std::vector<Block*> blocks_;
	// Tile-map used for drawing the blocks
	sf::VertexArray vertices_;
#pragma endregion
};
