#pragma once
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"

class World;
struct WorldSettings;

namespace gui
{
	void ShowGenerationSettings(World& world);
	void ShowGeneralSettings(WorldSettings& world);
	void ShowSurfaceSettings(WorldSettings& world);
	void ShowWaterSettings(WorldSettings& world);

	void HelpMarker(const char* desc);
	void CenterText(std::string text);
}
