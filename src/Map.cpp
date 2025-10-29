#include "Map.h"

#include <fstream>
#include <iostream>
#include <string>
#include <limits>

using namespace Cursed3DEngine;

Map::Map() : width(16), height(16)
{
    layout += L"################";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"#..............#";
	layout += L"################";
}

Map::Map(int width, int height, std::wstring layout) : width(width), height(height), layout(layout) {}

Map::Map(std::string mapFile)
{
	std::ifstream file(mapFile);
	if (!file) {
        throw std::runtime_error("Failed to open map file: " + mapFile);
    }

	// Read width and height
    if (!(file >> width)) throw std::runtime_error("Failed to read width");
    if (!(file >> height)) throw std::runtime_error("Failed to read height");

	// Consume the leftover newline
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	// Read lines
	for (int i = 0; i < height; ++i) {
		std::string line;
		std::getline(file, line);

		if ((int)line.size() != width) {
			throw std::runtime_error(
                "Line " + std::to_string(i+1) + " has wrong length: " + std::to_string(line.size()));
		}

		layout += std::wstring(line.begin(), line.end());
	}
}

Map::~Map() {}

int Map::getWidth() const
{
	return width;
}

int Map::getHeight() const
{
	return height;
}

std::wstring Map::getLayout() const
{
    return layout;
}

wchar_t Map::operator[](int index) const
{
	return layout[index];
}