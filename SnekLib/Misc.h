#pragma once

#include <cmath>

struct Coord
{
		unsigned x, y;
		auto operator<=>(const Coord&) const = default;
		
		int ManhattanDist(Coord other) const
		{
				return std::abs((int)x - (int)other.x) + std::abs((int)y - (int)other.y);
		}
};

struct Treat
{
		Coord Coord;
		int Owner = -1;
		auto operator<=>(const Treat&) const = default;
};

enum Dir
{
		None, Left, Right, Up, Down
};
