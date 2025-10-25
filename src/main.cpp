#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <locale.h>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;
int nMapWidth = 16;
int nMapHeight = 16;

float fPlayerX = 14.7f;
float fPlayerY = 5.09f;
float fPlayerA = 0.0f;
float fFOV = 3.14159f / 4.0f;
float fDepth = 16.0f;
float fSpeed = 10.0f;

int main()
{
    setlocale(LC_ALL, "");

    WINDOW *mainWin = initscr();
    if (mainWin == NULL) {
		printf("Failed to initialize ncursesw. Exiting...\n");
		exit(EXIT_FAILURE);
	}

    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(0);

    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(0);

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

    resize_term(nScreenHeight, nScreenWidth);

    unique_ptr<wchar_t[]> screen = std::make_unique<wchar_t[]>(nScreenWidth * nScreenHeight);

	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#.......########";
	map += L"#..............#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#..............#";
	map += L"###............#";
	map += L"##.............#";
	map += L"#......####..###";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = tp1;
	float clearTime = 0.0f;
	
    int pressedKey = 0;
	for (;;) {
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();
		clearTime += fElapsedTime;

        pressedKey = getch();
        if (pressedKey != ERR) {
            pressedKey = toupper(pressedKey);
        }

        if (pressedKey == 'Q') {
            break;
        }

		// rotate counter clock wise
		if (pressedKey == 'A') {
			fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;
		}

		// rotate clock wise
		if (pressedKey == 'D') {
			fPlayerA += (fSpeed * 0.75f) * fElapsedTime;
		}
		
		// move fwd
		if (pressedKey == 'W') {
			fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#') {
				fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}			
		}

		// move bwd
		if (pressedKey == 'S') {
			fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#') {
				fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}

		if (clearTime >= 0.1f) {
			clearok(stdscr, TRUE);
			clearTime = 0.0f;
		}

		erase();

		for (int x = 0; x < nScreenWidth; x++) {
			float fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fStepSize = 0.001f;									
			float fDistanceToWall = 0.0f;

			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth) {
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
				
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = fDepth;
				} else {
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#') {
						bHitWall = true;

						vector<pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++) {
							for (int ty = 0; ty < 2; ty++) {
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx*vx + vy*vy); 
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}
						}

						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });
						
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						if (acos(p.at(2).second) < fBound) bBoundary = true;
					}
				}
			}
		
			int nCeiling = (float)(nScreenHeight/2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;
			
			for (int y = 0; y < nScreenHeight; y++) {
				wchar_t nShade = L' ';
				int nColor = 4;

				if (y <= nCeiling) {
					// Sky
					nShade = L' ';
					nColor = 4;
				}
				else if (y > nCeiling && y <= nFloor) {
					// Walls — Unicode shaded blocks
					if (fDistanceToWall <= fDepth / 4.0f)       { nShade = 0x2588; nColor = 1; } // █ — very close
					else if (fDistanceToWall < fDepth / 3.0f)   { nShade = 0x2593; nColor = 2; } // ▓
					else if (fDistanceToWall < fDepth / 2.0f)   { nShade = 0x2592; nColor = 3; } // ▒
					else if (fDistanceToWall < fDepth)          { nShade = 0x2591; nColor = 4; } // ░
					else                                        { nShade = L' ';   nColor = 4; }

					if (bBoundary) {
						nShade = 0x2592;
						nColor = 8; // outline color
					}
				}
				else {
					// Floor — Unicode shading gradient
					float b = ((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f);
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

		// Minimap
        for (int nx = 0; nx < nMapWidth; nx++) {
            for (int ny = 0; ny < nMapHeight; ny++) {
                mvaddch(ny + 1, nx, map[ny * nMapWidth + nx]);
            }
        }
        mvaddch((int)fPlayerX + 1, (int)fPlayerY, 'P');

		// HUD
        mvprintw(0, 0, "X=%3.2f Y=%3.2f A=%3.2f FPS=%3.2f   ", fPlayerX, fPlayerY, fPlayerA, 1.0f/fElapsedTime);

        refresh();
        this_thread::sleep_for(std::chrono::milliseconds(16)); // limit to ~60 FPS

	}

    endwin();

	return 0;
}