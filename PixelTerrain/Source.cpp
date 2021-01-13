#pragma warning (disable: 4244)

#include "GenerationTasks.h"
#include "Gui.h"

int main()
{
	// Setup window
    sf::Vector2i window_size(1280, 640);
    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Pixel Terrain");
    ImGui::SFML::Init(window);

    // Setup view
    sf::View main_view(sf::FloatRect(
        0, 0,
        window_size.x, window_size.y
    ));
    window.setView(main_view);
	
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
    	40, 250, 5,
        6, 50,
        1, 2, 10 , 20,
    	2, 8
    }, 10);
	
	// Add all generation steps and generate the world
    world->AddGenerationTask(generation_tasks::HeightMap);
    world->AddGenerationTask(generation_tasks::Overhangs);
    world->AddGenerationTask(generation_tasks::Water);
    world->AddGenerationTask(generation_tasks::Trees);
    world->Generate();

    // Game loop variables
    sf::Clock delta_clock;
    float delta_time;
    bool show_settings = true;

    float view_move_speed = 300;

    Block* active_block = blocks::dirt;
    int brush_size = 4;

    sf::Vector2i mouse_pos;
    bool left_mouse_button_held = false;
    bool right_mouse_button_held = false;
	
	// Game loop
    while (window.isOpen())
    {
        delta_time = delta_clock.getElapsedTime().asSeconds();
    	
		// Event handling
        sf::Event event;
        ImGuiIO io = ImGui::GetIO();
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (io.WantCaptureKeyboard) break;
                
                switch (event.key.code)
                {
                // Toggle visibility of generation settings
                case sf::Keyboard::Escape:
                    show_settings = !show_settings;
                    break;
                
                default:
                    break;
                }
                    
                break;

            case sf::Event::MouseButtonPressed:
                if (io.WantCaptureMouse) break;
                
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                    left_mouse_button_held = true;

                if (event.mouseButton.button == sf::Mouse::Button::Right)
                    right_mouse_button_held = true;
                break;

            case sf::Event::MouseButtonReleased:
                if (io.WantCaptureMouse) break;
                
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                    left_mouse_button_held = false;

                if (event.mouseButton.button == sf::Mouse::Button::Right)
                    right_mouse_button_held = false;
                break;

            case sf::Event::MouseWheelScrolled:
                if (io.WantCaptureMouse) break;
                
                // Zoom the view
                if (maths::Sign(event.mouseWheelScroll.delta) == 1)
                    main_view.zoom(0.95);
                else if (maths::Sign(event.mouseWheelScroll.delta) == -1)
                    main_view.zoom(1.05);
                break;

            default:
                break;
            }
        }

    	// Transform view
    	if (!io.WantCaptureKeyboard)
    	{
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                main_view.move(0, -view_move_speed * delta_time);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                main_view.move(-view_move_speed * delta_time, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                main_view.move(0, view_move_speed * delta_time);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                main_view.move(view_move_speed * delta_time, 0);

            window.setView(main_view);
    	}

        // Update mouse buttons
    	if (io.WantCaptureKeyboard)
    	{
            left_mouse_button_held = false;
            right_mouse_button_held = false;
    	}
    	
    	// Update mouse position in terms of block position
    	sf::Vector2f window_mouse_pos = window.mapPixelToCoords(
            sf::Mouse::getPosition(window
        ));
        mouse_pos = sf::Vector2i(
            window_mouse_pos.x,
            window_mouse_pos.y
        );
    	
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

        world->Update(window);
        ImGui::SFML::Update(window, delta_clock.restart());

    	// GUI
        gui::ShowBrushSettings(window_size, brush_size, &active_block);
        gui::ShowViewSettings(view_move_speed, main_view.getCenter());
    	
    	if (show_settings)
			gui::ShowWorldSettings(window_size, *world);

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
