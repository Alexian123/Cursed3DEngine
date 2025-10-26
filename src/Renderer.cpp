#include "Renderer.h"

#include <locale.h>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace Cursed3DEngine;

Renderer::Renderer(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), oldPlayerPos(Vector2F())
{
    setlocale(LC_ALL, "");

    window = initscr();
    if (window == NULL)
        throw std::runtime_error("Failed to initialize ncurses screen");

    resize_term(screenHeight, screenWidth);
    initializeColors();
}

Renderer::~Renderer()
{
    endwin();
}

void Renderer::renderScene(float elapsedTime, const Player &player, const Map &map)
{
    const Vector2F &playerPos = player.getPosition();
    float playerRotationAngle = player.getRotationAngle();
    if (oldPlayerPos != playerPos || oldPlayerRotationAngle != playerRotationAngle) {
        clearok(stdscr, TRUE);
        oldPlayerPos = playerPos;
        oldPlayerRotationAngle = playerRotationAngle;
    }
    erase();


    renderWorld(map, player);
    renderMap(map, player);
    renderHUD(player, elapsedTime);
    refresh();
}

void Renderer::initializeColors()
{
    if (has_colors()) {
        start_color();
        use_default_colors();

        // Gray stone walls
        init_color(50, 700, 700, 700);  // light gray
        init_color(51, 400, 400, 400);  // mid gray
        init_color(52, 150, 150, 150);  // dark gray

        // Green floor (grass)
        init_color(53, 400, 1000, 400); // light green
        init_color(54, 200, 500, 200);  // dark green

        // Blue sky
        init_color(55, 200, 400, 1000); // strong blue

        // Wall gradient
        init_pair(1, 50,  -1); // light gray
        init_pair(2, 51,  -1); // mid gray
        init_pair(3, 52,  -1); // dark gray
        init_pair(4, COLOR_BLACK, -1); // boundary

        // Floor gradient
        init_pair(5, 53,  -1); // light green
        init_pair(6, 54,  -1); // dark green

        // Sky (solid)
        init_pair(7, 55, -1);
    }
}

void Renderer::renderWorld(const Map &map, const Player &player)
{
    const Vector2F &playerPos = player.getPosition();
    float rotationAngle = player.getRotationAngle();
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    for (int x = 0; x < screenWidth; x++) {
        float rayAngle = (rotationAngle - FOV / 2.0f) + ((float)x / (float)screenWidth) * FOV;

        float stepSize = 0.01f;
        float distanceToWall = 0.0f;

        bool hitWall = false;
        bool hitWallBoundary = false;

        Vector2F eye(sinf(rayAngle), cosf(rayAngle));

        while (!hitWall && distanceToWall < RAY_DEPTH) {
            distanceToWall += stepSize;
            int testX = (int)(playerPos.x + eye.x * distanceToWall);
            int testY = (int)(playerPos.y + eye.y * distanceToWall);

            if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight) {
                hitWall = true;
                distanceToWall = RAY_DEPTH;
            } else {
                if (map[testX * mapWidth + testY] == MapSymbol::WALL_SYMBOL) {
                    hitWall = true;

                    std::vector<std::pair<float, float>> p;
                    for (int tx = 0; tx < 2; tx++) {
                        for (int ty = 0; ty < 2; ty++) {
                            Vector2F v((float)testX + tx, (float)testY + ty);
                            v -= playerPos;
                            float d = v.normalize();
                            float dot = eye.dotProduct(v);
                            p.push_back(std::make_pair(d, dot));
                        }
                    }

                    std::sort(p.begin(), p.end(), [](auto &a, auto &b) {
                        return a.first < b.first;
                    });

                    float bound = 0.01f;
                    if (acos(p.at(0).second) < bound) hitWallBoundary = true;
                    if (acos(p.at(1).second) < bound) hitWallBoundary = true;
                    if (acos(p.at(2).second) < bound) hitWallBoundary = true;
                }
            }
        }

        int ceilling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
        int floor = screenHeight - ceilling;

        for (int y = 0; y < screenHeight; y++) {
            wchar_t shade = L' ';
            int color = 7; // default to sky

            if (y <= ceilling) {
                // --- Sky (solid strong blue) ---
                shade = 0x2593;
                color = 7;
            } else if (y > ceilling && y <= floor) {
                // --- Walls (stone gray gradient) ---
                if (hitWallBoundary) {
                    shade = 0x2593;
                    color = 4;
                } else if (distanceToWall <= RAY_DEPTH / 4.0f) {
                    shade = 0x2588; color = 1;
                } else if (distanceToWall < RAY_DEPTH / 3.0f) {
                    shade = 0x2593; color = 2;
                } else {
                    shade = 0x2592; color = 3;
                }
                if (rand() % 32 == 0)
	                shade = 0x2591 + (rand() % 3);
            } else {
                // --- Floor (grass gradient) ---
                float b = ((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f);
                if (b < 0.5) {
                    shade = 0x2588; color = 6;
                } else {
                    shade = 0x2592; color = 5;
                }
            }

            attron(COLOR_PAIR(color));
            cchar_t wc;
            setcchar(&wc, &shade, A_NORMAL, color, nullptr);
            mvadd_wch(y, x, &wc);
            attroff(COLOR_PAIR(color));
        }
    }
}

void Renderer::renderMap(const Map &map, const Player &player)
{
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    for (int nx = 0; nx < mapWidth; nx++) {
        for (int ny = 0; ny < mapHeight; ny++) {
            mvaddch(ny, screenWidth - mapWidth + nx, map[ny * mapWidth + nx]);
        }
    }

    const Vector2F &playerPos = player.getPosition();
    mvaddch((int)playerPos.x, screenWidth - mapWidth + (int)playerPos.y, PLAYER_SYMBOL);
}

void Renderer::renderHUD(const Player &player, float elapsedTime)
{
    const Vector2F &playerPos = player.getPosition();
    mvprintw(0, 0, "X=%3.2f Y=%3.2f A=%3.2f FPS=%3.2f   ",
             playerPos.x, playerPos.y,
             player.getRotationAngle(), 1.0f / elapsedTime);
}
