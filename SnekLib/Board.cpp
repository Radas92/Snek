#include "Board.h"

void Board::Init()
{
		unsigned spacing = Cols / (Sneks.size() + 1);
		for (unsigned i = 0; i < Sneks.size(); ++i)
		{
				Sneks[i]->Reset({ (i + 1) * spacing, Rows / 2 });
		}
}

void Board::Update()
{
		while (Treats.size() < TreatCount)
		{
				Treat t;
				t.Coord = { rand() % Cols, 	rand() % Rows };
				while (!IsFree(t.Coord)) 
				{
						t.Coord = GetNext(t.Coord);
				}

				Treats.insert(t);
		}
}

bool Board::IsFree(Coord c)
{
		if (std::find_if(Treats.begin(), Treats.end(), [&](auto& t) {return t.Coord == c; }) != Treats.end())
		{
				return false;
		}

		for (auto& s : Sneks)
		{
				if (s->LastIndex(c) >= 0)
				{
						return false;
				}
		}

		return true;
}

bool Board::IsWithinBounds(Coord c)
{
		return c.x >= 0 && c.x < Cols&& c.y >= 0 && c.y < Rows;
}

Coord Board::GetNext(Coord c)
{
		auto ret = c;
		if (c.x < Cols - 1) 
		{
				++ret.x;
		}
		else 
		{
				ret.x = 0;
				++ret.y;
				ret.y %= Rows;
		}

		return ret;
}