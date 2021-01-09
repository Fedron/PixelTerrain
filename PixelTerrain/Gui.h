#pragma once
#include <string>

#include "imgui.h"
#include "imgui-SFML.h"

class World;

namespace gui
{
	void ShowGenerationSettings(World& world);
	void ShowGeneralSettings(World& world);
	void ShowSurfaceSettings(World& world);
	void ShowWaterSettings(World& world);

	void HelpMarker(const char* desc);
	void CenterText(std::string text);
}
