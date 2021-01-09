#include "Gui.h"

#include "World.h"

namespace gui
{
	void ShowGenerationSettings(World& world)
	{
		if (!ImGui::Begin("Generation Settings"))
		{
			ImGui::End();
			return;
		}

		static bool random_seed = true;
		ImGui::Checkbox("Use random seed", &random_seed);
		
		static int seed = time(nullptr);
		if (!random_seed)
			ImGui::InputInt("Seed", &seed);
		
		ImGui::Spacing();
		
		ShowGeneralSettings(world);
		ShowSurfaceSettings(world);
		ShowWaterSettings(world);

		ImGui::Spacing(); ImGui::Spacing();

		const int window_width = ImGui::GetWindowSize().x;
		ImGui::Button("Generate", ImVec2(
			window_width,
			20
		));
		CenterText(std::string("World generated in 999ms"));
		
		ImGui::End();
	}

	void ShowGeneralSettings(World& world)
	{
		if (!ImGui::CollapsingHeader("General"))
			return;

		ImGui::SliderInt("Block size", &world.block_size_, 1, 32);
		ImGui::SameLine(); HelpMarker(
			"Size of an individual block in pixels"
		);
		
		ImGui::SliderInt("Chunk width", &world.chunk_width_, 4, 128);
		ImGui::SameLine(); HelpMarker(
			"Width of every chunk in blocks"
		);
		
		ImGui::SliderInt("Chunk height", &world.chunk_height_, 4, 128);
		ImGui::SameLine(); HelpMarker(
			"Height of every chunk in blocks"
		);

		ImGui::SliderInt("No. chunks X", &world.num_chunks_x_, 1, 128);
		ImGui::SameLine(); HelpMarker(
			"Number of chunks along the width of the world"
		);

		ImGui::SliderInt("No. chunks Y", &world.num_chunks_y_, 1, 128);
		ImGui::SameLine(); HelpMarker(
			"Number of chunks along the height of the world"
		);

		ImGui::Spacing();
	}
	
	void ShowSurfaceSettings(World& world)
	{
		if (!ImGui::CollapsingHeader("Height-map"))
			return;
		
		ImGui::Text("Height-map settings");

		// TODO: Add to configurable settings
		ImGui::SliderInt("Smoothness", &world.block_size_, 1, 32);
		ImGui::SameLine(); HelpMarker(
			"Controls how gradually transitions between peaks and troughs occur"
		);
		
		ImGui::DragIntRange2("Surface level", &world.min_surface_level_,
			&world.max_surface_level_, 1, 0,
			world.num_chunks_y_ * world.chunk_height_);
		ImGui::SameLine(); HelpMarker(
			"Range in which the surface can generate"
		);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Extra detail");

		// TODO: Add to configurable settings
		ImGui::SliderInt("Surface roughness", &world.block_size_, 1, 32);
		ImGui::SameLine(); HelpMarker(
			"Controls how often overhangs and indents are made"
		);

		// TODO: Add to configurable settings
		ImGui::SliderInt("Overhang amount", &world.block_size_, 1, 32);
		ImGui::SameLine(); HelpMarker(
			"How far out an overhang can go"
		);

		ImGui::Spacing();
	}

	void ShowWaterSettings(World& world)
	{
		if (!ImGui::CollapsingHeader("Water"))
			return;

		// TODO: Add to configurable settings
		static bool generate_water = false;
		ImGui::Checkbox("Generate water", &generate_water);
		ImGui::SameLine(); HelpMarker(
			"Determines if lakes should be generated"
		);

		// TODO: Add to configurable settings
		static bool random_sea_level = false;
		ImGui::Checkbox("Random sea-level", &random_sea_level);
		ImGui::SameLine(); HelpMarker(
			"Whether or not to use a pre-determined sea-level"
		);

		// TODO: Add to configurable settings
		static int min_sea_level = world.min_surface_level_;
		static int max_sea_level = world.max_surface_level_;
		if (random_sea_level)
		{
			ImGui::DragIntRange2("Sea-level range", &min_sea_level,
				&max_sea_level, 1, world.min_surface_level_,
				world.max_surface_level_);
			ImGui::SameLine(); HelpMarker(
				"Range in which the sea-level can be randomly chosen"
			);
		}

		// TODO: Add to configurable settings
		static int sand_range = 10;
		ImGui::SliderInt("Sand range", &sand_range, 1, 32);
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
