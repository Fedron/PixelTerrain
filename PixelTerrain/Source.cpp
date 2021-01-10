#include "GenerationTasks.h"
#include "Gui.h"

int main()
{
    sf::Vector2i window_size(1280, 640);
	
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Pixel Terrain");
    ImGui::SFML::Init(window);
	
	// Create world
    auto* world = new World(WorldSettings{
        static_cast<int>(time(nullptr)),
    	32, 32,
    	20, 11,
    	2,
    	1,
    	50, 320,
    	5, 20,
    	10, 30,
    	true,
    	40, 250, 5
    });
	
	// Add all generation steps and generate the world
    world->AddGenerationTask(generation_tasks::HeightMap);
    world->AddGenerationTask(generation_tasks::Overhangs);
    world->AddGenerationTask(generation_tasks::Water);
    world->Generate();

    // Game loop variables
    sf::Clock delta_clock;
    bool show_settings = true;

    Block active_block = blocks::dirt;
    int brush_size = 4;

    sf::Vector2i mouse_pos;
    bool left_mouse_button_held = false;
    bool right_mouse_button_held = false;
	
	// Game loop
    while (window.isOpen())
    {
		// Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // Toggle visibility of generation settings
                if (event.key.code == sf::Keyboard::Escape)
                    show_settings = !show_settings;
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                    left_mouse_button_held = true;

                if (event.mouseButton.button == sf::Mouse::Button::Right)
                    right_mouse_button_held = true;
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                    left_mouse_button_held = false;

                if (event.mouseButton.button == sf::Mouse::Button::Right)
                    right_mouse_button_held = false;
                break;

            default:
                break;
            }
        }

        mouse_pos = sf::Mouse::getPosition(window);
    	// Convert window pixels to world coordinates
        mouse_pos.x /= 2;
        mouse_pos.y /= 2;

    	// Updates
    	// Place blocks
        if (left_mouse_button_held)
        {
            for (int x = mouse_pos.x - brush_size; x < mouse_pos.x + brush_size; x++)
            {
                for (int y = mouse_pos.y - brush_size; y < mouse_pos.y + brush_size; y++)
                {
                    world->SetBlock(x, world->world_height_ - y, active_block);
                }
            }
        }

    	// Delete blocks
        if (right_mouse_button_held)
        {
            for (int x = mouse_pos.x - brush_size; x < mouse_pos.x + brush_size; x++)
            {
                for (int y = mouse_pos.y - brush_size; y < mouse_pos.y + brush_size; y++)
                {
                    world->SetBlock(x, world->world_height_ - y, blocks::air);
                }
            }
        }
    	
        ImGui::SFML::Update(window, delta_clock.restart());

    	// GUI
        gui::ShowBrushSettings(window_size, brush_size, active_block);
    	
    	if (show_settings)
			gui::ShowGenerationSettings(window_size, *world);

        //ImGui::ShowDemoWindow();
    	
    	// Draw calls
        window.clear(sf::Color(182, 227, 219, 255));
        world->Draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    delete world;
	
    ImGui::SFML::Shutdown();
    return 0;
}
