#include "Map.h"

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