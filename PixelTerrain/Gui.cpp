#pragma warning (disable: 4244 4305)

#include "Gui.h"
#include "World.h"

namespace gui
{
	void ShowBrushSettings(sf::Vector2i& window_size, int& brush_size, Block** active_block)
	{
		const ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
		
		static bool open = true;
		if (!ImGui::Begin("Brush Settings", &open, flags))
		{
			ImGui::End();
			return;
		}
		
		ImGui::Text("Brush size");
		ImGui::SliderInt("", &brush_size, 1, 20);

		ImGui::Spacing();

		ImGui::Text("Active block");
		const char* blocks[] = { "Dirt", "Grass", "Stone", "Sand", "Water", "Wood", "Leaf" };
		static int selected_block = 0;
		ImGui::Combo(" ", &selected_block, blocks, IM_ARRAYSIZE(blocks));

		// Set active_block depending on selected_block
		if (selected_block == 0)
			*active_block = blocks::dirt;
		else if (selected_block == 1)
			*active_block = blocks::grass;
		else if (selected_block == 2)
			*active_block = blocks::stone;
		else if (selected_block == 3)
			*active_block = blocks::sand;
		else if (selected_block == 4)
			*active_block = blocks::water;
		else if (selected_block == 5)
			*active_block = blocks::wood;
		else if (selected_block == 6)
			*active_block = blocks::leaf;
		
		ImGui::End();
	}

	void ShowViewSettings(float& move_speed, const sf::Vector2f view_center)
	{
		const ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;

		static bool open = true;
		if (!ImGui::Begin("View Settings", &open))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Move Speed");
		ImGui::SliderFloat("", &move_speed, 0, 5000, "%.1f");

		ImGui::Spacing();

		ImGui::Text(
			std::string("View Center: " +\
				std::to_string(static_cast<int>(view_center.x)) +\
				", " +\
				std::to_string(static_cast<int>(view_center.y))).c_str()
		);

		ImGui::End();
	}

	void ShowWorldSettings(sf::Vector2i& window_size, World& world)
	{
		ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0), ImVec2(-1, FLT_MAX));

		if (!ImGui::Begin("Generation Settings"))
		{			
			ImGui::End();
			return;
		}
		
		// Seed settings
		static bool random_seed = true;
		ImGui::Checkbox("Use random seed", &random_seed);
		
		static int seed = time(nullptr);
		if (!random_seed)
			ImGui::InputInt("Seed", &seed);
		
		ImGui::Spacing();

		const int greater_num = world.gen_settings_.num_chunks_x < world.gen_settings_.num_chunks_y ?
			world.gen_settings_.num_chunks_y : world.gen_settings_.num_chunks_x;
		ImGui::SliderInt("Render Range", &world.render_range_, 1, greater_num);
		ImGui::SameLine(); HelpMarker(
			"Half-extends of the square in which chunks are rendered"
		);
		
		ImGui::Spacing();

		// Show all the generation sections
		ShowGeneralSettings(world.gen_settings_);
		ShowSurfaceSettings(world.gen_settings_);
		ShowWaterSettings(world.gen_settings_);
		ShowTreeSettings(world.gen_settings_);

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

	void ShowTreeSettings(WorldSettings& world)
	{
		if (!ImGui::CollapsingHeader("Trees"))
			return;

		ImGui::SliderInt("Tree spacing", &world.tree_spacing, 1, world.chunk_width);
		ImGui::SameLine(); HelpMarker(
			"Determines the amount of air blocks in-between trees"
		);
		
		ImGui::SliderInt("Trees per chunk", &world.trees_per_chunk, 1, world.chunk_width);
		ImGui::SameLine(); HelpMarker(
			"The maximum amount of trees in a given chunk"
		);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Individual tree settings");

		ImGui::DragIntRange2("Trunk width", &world.min_trunk_width,
			&world.max_trunk_width, 1, 1, 50);
		ImGui::SameLine(); HelpMarker(
			"Thickness of the trunk, in blocks on each side of the center"
		);

		ImGui::DragIntRange2("Trunk height", &world.min_trunk_height,
			&world.max_trunk_height, 1, world.max_treetop_size, 100);
		ImGui::SameLine(); HelpMarker(
			"The height of the trunk in blocks"
		);

		ImGui::DragIntRange2("Treetop size", &world.min_treetop_size,
			&world.max_treetop_size, 1, 0, 50);
		ImGui::SameLine(); HelpMarker(
			"The half-extends of the treetops"
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
