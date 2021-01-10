#pragma warning (disable: 4244 4305)

#include "Gui.h"
#include "World.h"

namespace gui
{
	void ShowGenerationSettings(World& world)
	{
		const ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_MenuBar;

		static bool p_open = false;
		if (!ImGui::Begin("Generation Settings", &p_open, flags))
		{
			ImGui::End();
			return;
		}

		// Window setup
		ImGui::SetWindowPos(ImVec2(
			 1280- ImGui::GetWindowWidth(),
			0
		));
		ImGui::SetWindowSize(ImVec2(
			500, 640)
		);

		// Seed settings
		static bool random_seed = true;
		ImGui::Checkbox("Use random seed", &random_seed);
		
		static int seed = time(nullptr);
		if (!random_seed)
			ImGui::InputInt("Seed", &seed);
		
		ImGui::Spacing();

		// Show all the sections
		ShowGeneralSettings(world.settings_);
		ShowSurfaceSettings(world.settings_);
		ShowWaterSettings(world.settings_);

		ImGui::Spacing(); ImGui::Spacing();

		// Generate button and generation time
		if (ImGui::Button("Generate", ImVec2(
			ImGui::GetWindowWidth(),
			20
		)))
		{
			world.Generate();
		}
		CenterText(std::string("World generated in " + std::to_string(world.generation_time_) + "ms"));
		
		ImGui::End();
	}

	void ShowGeneralSettings(WorldSettings& world)
	{
		if (!ImGui::CollapsingHeader("General"))
			return;

		ImGui::SliderInt("Block size", &world.block_size, 1, 32);
		ImGui::SameLine(); HelpMarker(
			"Size of an individual block in pixels"
		);
		
		ImGui::SliderInt("Chunk width", &world.chunk_width, 4, 128);
		ImGui::SameLine(); HelpMarker(
			"Width of every chunk in blocks"
		);
		
		ImGui::SliderInt("Chunk height", &world.chunk_height, 4, 128);
		ImGui::SameLine(); HelpMarker(
			"Height of every chunk in blocks"
		);

		ImGui::SliderInt("No. chunks X", &world.num_chunks_x, 1, 128);
		ImGui::SameLine(); HelpMarker(
			"Number of chunks along the width of the world"
		);

		ImGui::SliderInt("No. chunks Y", &world.num_chunks_y, 1, 128);
		ImGui::SameLine(); HelpMarker(
			"Number of chunks along the height of the world"
		);
		
		ImGui::Spacing();
	}
	
	void ShowSurfaceSettings(WorldSettings& world)
	{
		if (!ImGui::CollapsingHeader("Surface"))
			return;
		
		ImGui::Text("Height-map settings");

		ImGui::SliderFloat("Smoothness", &world.surface_smoothness, 0.1, 7);
		ImGui::SameLine(); HelpMarker(
			"Controls how gradually transitions between peaks and troughs occur"
		);
		
		ImGui::DragIntRange2("Surface level", &world.min_surface_level,
			&world.max_surface_level, 1, 0,
			world.num_chunks_y * world.chunk_height);
		ImGui::SameLine(); HelpMarker(
			"Range in which the surface can generate"
		);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Block Layer");

		ImGui::SliderInt("Grass Thickness", &world.grass_layer_thickness, 1, 16);
		ImGui::SameLine(); HelpMarker(
			"Thickness of the grass layer"
		);

		ImGui::SliderInt("Dirt Thickness", &world.dirt_layer_thickness, 1, 16);
		ImGui::SameLine(); HelpMarker(
			"Thickness of the dirt layer"
		);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Extra detail");

		ImGui::SliderFloat("Surface roughness", &world.overhang_roughness, 0.1, 30);
		ImGui::SameLine(); HelpMarker(
			"Controls how often overhangs and indents are made"
		);

		ImGui::SliderInt("Overhang amount", &world.overhang_amount, 1, world.chunk_width * 3);
		ImGui::SameLine(); HelpMarker(
			"How far out an overhang can go"
		);

		ImGui::Spacing();
	}

	void ShowWaterSettings(WorldSettings& world)
	{
		if (!ImGui::CollapsingHeader("Water"))
			return;

		ImGui::Checkbox("Generate water", &world.generate_water);
		ImGui::SameLine(); HelpMarker(
			"Determines if lakes should be generated"
		);

		if (!world.generate_water) return;

		ImGui::DragIntRange2("Sea-level range", &world.min_sea_level,
			&world.max_sea_level, 1, world.min_surface_level,
			world.chunk_height * world.num_chunks_y);
		ImGui::SameLine(); HelpMarker(
			"Range in which the sea-level can be randomly chosen"
		);

		ImGui::SliderInt("Sand range", &world.sand_range, 1, 32);
		ImGui::SameLine(); HelpMarker(
			"Controls how far away sand can generate from a body of water"
		);

		ImGui::Spacing();
	}

	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void CenterText(std::string text)
	{
		ImGui::NewLine();
		
		const float font_size = ImGui::GetFontSize() * text.size() / 2;
		ImGui::SameLine(
			ImGui::GetWindowSize().x / 2 -
			font_size + (font_size / 2)
		);

		ImGui::Text(text.c_str());
	}
}
