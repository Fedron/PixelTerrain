# Pixel Terrain
Pixel terrain is a hobby project in which I play around with creating world generation, similar to what you find in [Terraria](https://store.steampowered.com/app/105600/Terraria/). This isn't intended to be a game you can play but instead just an application in which you can generate random worlds and place and delete blocks.

![Example of the demo in action](https://i.imgur.com/FptXDyS.png)
![Example of the demo in action](https://i.imgur.com/oYe3LrT.png)

## Features
- Random world generation
  - Surface generation
  - Random tree generation
  - Lake generation
- Placing and deleting blocks
- Starts of block simulation
  - Currently only sand is sumulated (falls when nothing supports it)
- GUI allowing full customisation of the generation

## Building and running
Currently only the solution file for the project is on GitHub, as a result to be able to run the program you will need to build the solution file in Visual Studio.
(An executable file will be added to GitHub once the project is at a relatively complete state).

Once you have built the solution you can find the executable in either the ```Debug``` or ```Release``` folders in the root directory of the project.

## Future Developments
Below are several features that I either have planned or would like to implement at some point:
- Cave generation
- Multiple biome types
- Fluid simulation
  - Along with several other simulated blocks like fire, smoke, explosives etc
- Saving and loading both worlds and generation settings
- Lighting (Similar to the look of [Starbound's lighting](https://cdn.cloudflare.steamstatic.com/steam/apps/211820/ss_198c15c2c8500bd57479c53713b4fbedb32b101a.1920x1080.jpg?t=1599228095))
