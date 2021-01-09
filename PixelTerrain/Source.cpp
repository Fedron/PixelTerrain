#include "Gui.h"
#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(1280, 640), "Pixel Terrain");
    ImGui::SFML::Init(window);
	
	// Create world
	// TODO: Make world calculate the chunk_size based on num_chunks and block_size
    auto* world = new World(
        32, 32, 20, 10, 2, 50, 320, 8, 30
    );
    //world->debug_mode_ = true;
	
	// Add all generation steps and generate the world
    world->AddGenerationTask(generation_tasks::HeightMap);
    world->AddGenerationTask(generation_tasks::Overhangs);
    world->AddGenerationTask(generation_tasks::Water);
    world->Generate();

	// Game loop
    sf::Clock delta_clock;
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
                // Regenerate world
                if (event.key.code == sf::Keyboard::Space)
                    world->Generate();
                break;

            default:
                break;
            }
        }

    	// Updates
        ImGui::SFML::Update(window, delta_clock.restart());

    	// GUI
        gui::ShowGenerationSettings(*world);
        //ImGui::ShowDemoWindow();
    	
    	// Draw calls
        window.clear(sf::Color(182, 227, 219, 255));
        world->Draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
