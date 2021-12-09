#pragma once

#include <queue>

#include "SnekGame.h"
#include "SFML/Window.hpp"
#include "DummyAi.h"

struct SnekAI_MLabut_RKrenek : public SnekAI
{
		virtual Team GetTeam() override { return Team::MLabutRKrenek; };

		struct Dirs
		{
			uint8_t data = 0;
			bool Has(Dir dir) { return (data & (1 << dir)) != 0; }
			void Set(Dir dir) { data = data ^ (1 << dir); }
		};

		//None, Left, Right, Up, Down
		static Coord Shift(Coord coord, Dir dir)
		{
			static int x[] = {0, -1, 1, 0, 0};
			static int y[] = {0, 0, 0, -1, 1};
			Coord newCoord;
			newCoord.x = coord.x + x[dir];
			newCoord.y = coord.y + y[dir];
			return newCoord;
		}

		static Dir Flip(Dir dir)
		{
			static Dir flipped[] = { None, Right, Left, Down, Up };
			return flipped[dir];
		}

		struct BoardWrapper
		{
			const Board& board;
			BoardWrapper(const Board& board) : board(board) {}

			bool ValidateDirection(const Snek& snek, Coord newPos)
			{
				if (!const_cast<Board&>(board).IsWithinBounds(newPos))
					return false;

				for (auto& snek : board.Sneks)
				{
					for (auto& bodyCoord : snek->Body)
					{
						if (bodyCoord.x == newPos.x && bodyCoord.y == newPos.y)
							return false;
					}
				}

				return true;
			}

			int GetDirectionPossibleSteps(const Snek& snek, const Coord& coord, const Dir& direction)
			{
				auto actualCoord = Shift(coord, direction);

				int possibleSteps = 0;
				while (ValidateDirection(snek, actualCoord))
				{
					++possibleSteps;
					actualCoord = Shift(actualCoord, direction);
				}

				return possibleSteps;
			}


			Dirs StepDirections(const Snek& snek, std::vector<Dir> myMoves)
			{
				Dirs dirs;
				for (Dir dir : {Left, Right, Up, Down})
				{
					Coord newPos = Shift(snek.Body[0], dir);

					if (!ValidateDirection(snek, newPos))
						continue;

					dirs.Set(dir);
				}
				return dirs;
			}
		};

		void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) override
		{
			BoardWrapper myBoard(board);
			Coord head = snek.Body[0];

			Dir bestDirection = Left;
			int bestDirectionPossibleSteps = myBoard.GetDirectionPossibleSteps(snek, head, Left);

			int possibleStepsRight = myBoard.GetDirectionPossibleSteps(snek, head, Right);
			if (possibleStepsRight > bestDirectionPossibleSteps)
			{
				bestDirectionPossibleSteps = possibleStepsRight;
				bestDirection = Right;
			}

			int possibleStepsUp = myBoard.GetDirectionPossibleSteps(snek, head, Up);
			if (possibleStepsUp > bestDirectionPossibleSteps)
			{
				bestDirectionPossibleSteps = possibleStepsUp;
				bestDirection = Up;
			}

			int possibleStepsDown = myBoard.GetDirectionPossibleSteps(snek, head, Down);
			if (possibleStepsDown > bestDirectionPossibleSteps)
			{
				bestDirectionPossibleSteps = possibleStepsDown;
				bestDirection = Down;
			}


			const int directionPenalty = 100 + snek.Body.size();
			auto it = std::min_element(board.Treats.begin(), board.Treats.end(),
				[&](Treat t1, Treat t2) 
				{ 
					auto t1distance = t1.Coord.ManhattanDist(head); 

					switch (bestDirection)
					{
					case Left:
					{
						if (t1.Coord.x > head.x)
						{
							t1distance += directionPenalty;
						}
						break;
					}
					case Right:
					{
						if (t1.Coord.x < head.x)
						{
							t1distance += directionPenalty;
						}
						break;
					}
					case Up:
					{
						if (t1.Coord.y > head.y)
						{
							t1distance += directionPenalty;
						}
						break;
					}
					case Down:
					{
						if (t1.Coord.y < head.y)
						{
							t1distance += directionPenalty;
						}
						break;
					}
					}

					return t1distance < t2.Coord.ManhattanDist(head);
				});

			Coord favorite = it->Coord;

			Dirs possible = myBoard.StepDirections(snek, {});

			if (head.x > favorite.x && bestDirection == Left) moveRequest = Left;
			else if (head.x < favorite.x && bestDirection == Right) moveRequest = Right;
			else if (head.y > favorite.y && bestDirection == Up) moveRequest = Up;
			else if (head.y < favorite.y && bestDirection == Down) moveRequest = Down;

			else if (head.x > favorite.x && possible.Has(Left)) moveRequest = Left;
			else if (head.x < favorite.x && possible.Has(Right)) moveRequest = Right;
			else if (head.y > favorite.y && possible.Has(Up)) moveRequest = Up;
			else if (head.y < favorite.y && possible.Has(Down)) moveRequest = Down;

			// Cannot go to favorite direction
			else 
			{
				if (possible.Has(bestDirection)) moveRequest = bestDirection;
				else if (possible.Has(Left)) moveRequest = Left;
				else if (possible.Has(Right)) moveRequest = Right;
				else if (possible.Has(Up)) moveRequest = Up;
				else if (possible.Has(Down)) moveRequest = Down;
			}
		}


		
};
