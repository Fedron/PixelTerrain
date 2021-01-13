#pragma once
#include <string>
#include "imgui-SFML.h"
#include "imgui.h"

class World;
class Block;
struct WorldSettings;

namespace gui
{
	/**
	 * Shows a panel for the brush settings
	 *
	 * @param window_size Size of the application window
	 * @param brush_size Reference to the brush size
	 * @param active_block Reference to the active block
	 */
	void ShowBrushSettings(sf::Vector2i& window_size, int& brush_size, Block** active_block);

	/**
	 * Shows settings for moving the view
	 *
	 * @param move_speed Reference to the move speed of the view
	 * @param view_center The center of the view
	 */
	void ShowViewSettings(float& move_speed, sf::Vector2f view_center);
	
	/**
	 * Shows the main ImGui panel
	 *
	 * @param world Reference to the world which the GUI belong to
	 * @param window_size Size of the application window
	 */
	void ShowWorldSettings(sf::Vector2i& window_size, World& world);

	/**
	 * Renders the general settings section
	 *
	 * @param world World settings for the world which to display settings for
	 */
	void ShowGeneralSettings(WorldSettings& world);

	/**
	 * Renders the surface generation settings section
	 *
	 * @param world World settings for the world which to display settings for
	 */
	void ShowSurfaceSettings(WorldSettings& world);

	/**
	 * Renders the water settings section
	 *
	 * @param world World settings for the world which to display settings for
	 */
	void ShowWaterSettings(WorldSettings& world);

	/**
	 * Renders the tree settings section
	 *
	 * @param world World settings for the world which to display settings for
	 */
	void ShowTreeSettings(WorldSettings& world);

	/**
	 * Displays a help icon in-line with a tooltip
	 *
	 * @param desc The tooltip to display on hover
	 */
	void HelpMarker(const char* desc);

	/**
	 * Renders text to ImGui in the center of the row
	 *
	 * @param text Text to render
	 */
	void CenterText(std::string text);
}
