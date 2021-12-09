#pragma once

#include "SnekGame.h"

inline void DummyStep(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost)
{
		Coord head = snek.Body[0];
		auto it = std::min_element(board.Treats.begin(), board.Treats.end(),
				[&](Treat t1, Treat t2) { return t1.Coord.ManhattanDist(head) < t2.Coord.ManhattanDist(head); }); //noice
		Coord favorite = it->Coord;
		if (head.x > favorite.x && snek.Heading() != Right) moveRequest = Left;
		if (head.x < favorite.x && snek.Heading() != Left) moveRequest = Right;
		if (head.y > favorite.y && snek.Heading() != Down) moveRequest = Up;
		if (head.y < favorite.y&& snek.Heading() != Up) moveRequest = Down;

		boost = head.ManhattanDist(favorite) > 9;
}
