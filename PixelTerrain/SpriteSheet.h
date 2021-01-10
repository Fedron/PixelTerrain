#pragma once
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

/**
 * Able to load several sprites from a single file
 */
class SpriteSheet
{
#pragma region Methods
public:
	/**
	 * Creates an empty sprite sheet
	 */
	SpriteSheet();

	/**
	 * Creates a sprite sheet and loads the texture
	 */
	explicit SpriteSheet(std::string filename);

	/**
	 * Deletes all sprite pointers
	 */
	~SpriteSheet();

	/**
	 * Loads a texture in the current directory with filename
	 *
	 * @param filename Name of the image file to load
	 */
	void LoadSpriteSheet(std::string filename);

	/**
	 * Creates an individual sprite from texture with the given rect
	 *
	 * @param name The name of the sprite (converted to all lowercase)
	 * @param left The left side of the sprite
	 * @param top The top side of the sprite
	 * @param width The width of the sprite
	 * @param height The height of the sprite
	 * @param scale_factor Scale factor of the sprite
	 */
	void CreateSprite(std::string name, int left, int top, int width, int height, int scale_factor = 1);

	/**
	 * Creates an individual sprite from texture with the given rect
	 *
	 * @param name The name of the sprite (converted to lowercase)
	 * @param rect The rect of the sprite
	 * @param scale_factor Scale factor of the sprite
	 */
	void CreateSprite(std::string name, sf::IntRect rect, int scale_factor = 1);

	/**
	 * Gets the sprite with the given name
	 *
	 * @param name Name of hte sprite to get
	 * @returns A pointer to the sprite
	 */
	sf::Sprite* GetSprite(std::string name);
#pragma endregion

#pragma region Variables
private:
	// Sprite sheet texture
	sf::Texture sprite_sheet_;
	// List of all the sprites, with a named key
	std::unordered_map<std::string, sf::Sprite*> sprites_;
#pragma endregion
};
