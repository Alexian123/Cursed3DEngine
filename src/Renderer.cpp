#include "Renderer.h"

#include <locale.h>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace Cursed3DEngine;

Renderer::Renderer(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
{
    setlocale(LC_ALL, "");

    window = initscr();
    if (window == NULL) {
		throw std::runtime_error("Failed to initialize screen");
	}

    resize_term(screenHeight, screenWidth);
    initializeColors();
}

Renderer::~Renderer()
{
    endwin();
}

void Renderer::renderScene(float elapsedTime, const Player &player, const Map& map)
{
    clearScreenTime += elapsedTime;
    if (clearScreenTime >= 0.1f) {
        clearok(stdscr, TRUE);
        clearScreenTime = 0.0f;
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
		init_pair(1, COLOR_WHITE,   -1); // close
		init_pair(2, COLOR_CYAN,    -1); // mid
		init_pair(3, COLOR_BLUE,    -1); // far
		init_pair(4, COLOR_BLACK,   -1); // very far
		init_pair(5, COLOR_GREEN,   -1); // floor (bright)
		init_pair(6, COLOR_YELLOW,  -1); // floor (mid)
		init_pair(7, COLOR_RED,     -1); // floor (dark)
		init_pair(8, COLOR_MAGENTA, -1); // wall boundary
    }
}

void Renderer::renderWorld(const Map& map, const Player &player)
{
    const Vector2F &playerPos = player.getPosition();
    float rotationAngle = player.getRotationAngle();
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();
    
    for (int x = 0; x < screenWidth; x++) {
        float rayAngle = (rotationAngle - FOV / 2.0f) + ((float)x / (float)screenWidth) * FOV;

        float stepSize = 0.001f;									
        float distanceToWall = 0.0f;

        bool hitWall = false;
        bool hitWallBoundary = false;

        Vector2F eye(sinf(rayAngle), cosf(rayAngle));

        while (!hitWall && distanceToWall < RAY_DEPTH) {
            distanceToWall += stepSize;
            int nTestX = (int)(playerPos.x + eye.x * distanceToWall);
            int nTestY = (int)(playerPos.y + eye.y * distanceToWall);
            
            if (nTestX < 0 || nTestX >= mapWidth || nTestY < 0 || nTestY >= mapHeight) {
                hitWall = true;
                distanceToWall = RAY_DEPTH;
            } else {
                if (map[nTestX * mapWidth + nTestY] == MapSymbol::WALL_SYMBOL) {
                    hitWall = true;

                    std::vector<std::pair<float, float>> p;

                    for (int tx = 0; tx < 2; tx++) {
                        for (int ty = 0; ty < 2; ty++) {
                            Vector2F v((float)nTestX + tx, (float)nTestY + ty);
                            v -= playerPos;
                            float d = v.normalize(); 
                            float dot = eye.dotProduct(v);
                            p.push_back(std::make_pair(d, dot));
                        }
                    }

                    std::sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {
                        return left.first < right.first; 
                    });
                    
                    float fBound = 0.01;
                    if (acos(p.at(0).second) < fBound) hitWallBoundary = true;
                    if (acos(p.at(1).second) < fBound) hitWallBoundary = true;
                    if (acos(p.at(2).second) < fBound) hitWallBoundary = true;
                }
            }
        }
    
        int nCeiling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
        int nFloor = screenHeight - nCeiling;
        
        for (int y = 0; y < screenHeight; y++) {
            wchar_t nShade = L' ';
            int nColor = 4;

            if (y <= nCeiling) {
                // Sky
                nShade = L' ';
                nColor = 4;
            }
            else if (y > nCeiling && y <= nFloor) {
                // Walls — Unicode shaded blocks
                if (distanceToWall <= RAY_DEPTH / 4.0f)       { nShade = 0x2588; nColor = 1; } // █ — very close
                else if (distanceToWall < RAY_DEPTH / 3.0f)   { nShade = 0x2593; nColor = 2; } // ▓
                else if (distanceToWall < RAY_DEPTH / 2.0f)   { nShade = 0x2592; nColor = 3; } // ▒
                else if (distanceToWall < RAY_DEPTH)          { nShade = 0x2591; nColor = 4; } // ░
                else                                        { nShade = L' ';   nColor = 4; }

                if (hitWallBoundary) {
                    nShade = 0x2592;
                    nColor = 8; // outline color
                }
            }
            else {
                // Floor — Unicode shading gradient
                float b = ((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f);
                if (b < 0.25)       { nShade = 0x2588; nColor = 5; } // █
                else if (b < 0.5)   { nShade = 0x2593; nColor = 6; } // ▓
                else if (b < 0.9)   { nShade = 0x2592; nColor = 7; } // ▒
                else                { nShade = 0x2591; nColor = 4; } // ░
            }

            if (nShade >= 0x2591 && nShade <= 0x2593 && rand() % 30 == 0)
                nShade = 0x2591 + (rand() % 3);  // random shade ░▒▓

            attron(COLOR_PAIR(nColor));
            cchar_t wc;
            setcchar(&wc, &nShade, A_NORMAL, nColor, nullptr);
            mvadd_wch(y, x, &wc);
            attroff(COLOR_PAIR(nColor));
        }
    }
}

void Renderer::renderMap(const Map &map, const Player &player)
{
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    for (int nx = 0; nx < mapWidth; nx++) {
            for (int ny = 0; ny < mapHeight; ny++) {
                mvaddch(ny + 1, nx, map[ny * mapWidth + nx]);
            }
    }
    const Vector2F &playerPos = player.getPosition();
    mvaddch((int)playerPos.x + 1, (int)playerPos.y, PLAYER_SYMBOL);
}

void Renderer::renderHUD(const Player &player, float elapsedTime)
{
    const Vector2F &playerPos = player.getPosition();
    mvprintw(0, 0, "X=%3.2f Y=%3.2f A=%3.2f FPS=%3.2f   ", 
        playerPos.x, playerPos.y, player.getRotationAngle(), 1.0f / elapsedTime);
}