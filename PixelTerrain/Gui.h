#pragma once
#include <string>
#include "imgui-SFML.h"
#include "imgui.h"

class World;
struct WorldSettings;

namespace gui
{
	/**
	 * Shows the main ImGui panel
	 *
	 * @param world Reference to the world which the GUI belong to
	 */
	void ShowGenerationSettings(World& world);

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
