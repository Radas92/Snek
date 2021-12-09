#include "Snek.h"
#include "Misc.h"
#include "Board.h"

void Snek::Move(Dir dir, Board& board, int frameNumber)
{
		Body.insert(Body.begin(), Body.front());

		auto& p = Body.front();

		switch (dir)
		{
		case Left:--p.x;
				break;
		case Right:++p.x;
				break;
		case Up:--p.y;
				break;
		case Down:++p.y;
				break;
		default:
				break;
		}

		auto it = std::find_if(board.Treats.begin(), board.Treats.end(), [&](auto& t) {return t.Coord == p; });
		if (it != board.Treats.end()) {
				board.Treats.erase(it);
		}
		else
		{
				Body.pop_back();
		}

		if (!board.IsWithinBounds(Body.front()))
		{
				Die(frameNumber);
		}

		for (auto& s : board.Sneks)
		{
				bool self = s->ID == ID;
				//if (self) continue;

				int index = s->LastIndex(Body.front());
				if (index > 0 || (!self && index == 0)) {
						Die(frameNumber);
				}
		}
}

void Snek::Reset(Coord headPos)
{
		for (int i = 0; i < MinimalLength; ++i)
		{
				Body.push_back(headPos);
				Body[i].y += i;
		}
}

int Snek::LastIndex(Coord c)
{
		auto it = std::find(Body.rbegin(), Body.rend(), c);
		if (it != Body.rend())
		{
				return std::distance(it, Body.rend()) - 1;
		}
		return -1;
}

void Snek::ReportStepDuration(sf::Int64 durationMicroseconds, int frameNumber, bool mt)
{
		AI.ReportStepDuration(durationMicroseconds);

		if (durationMicroseconds > (AI.MicrosecondsLimit * (mt ? 50 : 1)))
		{
				++m_Strikes;
				if (m_Strikes > 3) 
				{
						Die(frameNumber);
				}
		}
}

void Snek::Die(int frameNumber)
{
		m_Death = true;
		m_FrameOfDeath = frameNumber;
}
