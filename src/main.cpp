#include <iostream>
#include <chrono>
#include <thread>

#include "Renderer.h"
#include "KeyboardHandler.h"
#include "Map.h"
#include "Player.h"

using namespace Cursed3DEngine;

int main()
{
	Renderer renderer(120, 40);

	KeyboardHandler kbdHandler;
	
	std::wstring mapLayout;
	mapLayout += L"################";
	mapLayout += L"#..............#";
	mapLayout += L"#.......########";
	mapLayout += L"#..............#";
	mapLayout += L"#......##......#";
	mapLayout += L"#......##......#";
	mapLayout += L"#..............#";
	mapLayout += L"###............#";
	mapLayout += L"##.............#";
	mapLayout += L"#......####..###";
	mapLayout += L"#......#.......#";
	mapLayout += L"#......#.......#";
	mapLayout += L"#..............#";
	mapLayout += L"#......#########";
	mapLayout += L"#..............#";
	mapLayout += L"################";
	Map map(16, 16, mapLayout);

	Player player(14.7f, 5.09f, 0.0f);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = tp1;

	bool exit = false;
	while (!exit) {
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = tp2 - tp1;
		tp1 = tp2;
		float elapsedTime = delta.count();

		switch (kbdHandler.getPressedKey()) {
			case KBD_Q:
				exit = true;
				break;
			
			case KBD_W:
				player.applyMovement(MOVE_FORWARD, elapsedTime, map);
				break;
			
			case KBD_S:
				player.applyMovement(MOVE_BACKWARD, elapsedTime, map);
				break;

			case KBD_A:
				player.applyMovement(ROTATE_CCR, elapsedTime, map);
				break;

			case KBD_D:
				player.applyMovement(ROTATE_CR, elapsedTime, map);
				break;

			default:
				break;
		}

		renderer.renderScene(elapsedTime, player, map);

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // limit to ~60 FPS

	}

	return 0;
}