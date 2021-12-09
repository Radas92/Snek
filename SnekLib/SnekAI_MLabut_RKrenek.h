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

		struct MyBoard
		{
			const Board& board;
			MyBoard(const Board& board) : board(board) {}

			bool ValidateDirection(const Snek& snek, Coord newPos)
			{
				if (!const_cast<Board&>(board).IsWithinBounds(newPos))
					return false;

				for( auto& snek : board.Sneks )
				{
					for( auto& bodyCoord : snek->Body)
					{
						if (bodyCoord == newPos)
							return false;
					}
				}
				return true;
			}

			Dirs StepDirections(const Snek& snek, std::vector<Dir> myMoves)
			{
				Dirs dirs;
				for( Dir dir : {Left, Right, Up, Down} )
				{
					Coord newPos = Shift(snek.Body[0], dir);

					if (!ValidateDirection(snek, newPos))
						continue;

					dirs.Set(dir);
				}
				return dirs;
			}

			struct Node
			{
				std::vector<Dir> dirs;

				Coord finalCoord;
				uint16_t penalty;
				const Node* parent;

				Node(const Snek& snek, const Board& board, const Node* parent, const std::vector<Dir>& dirs) : dirs(dirs), parent(parent)
				{
					finalCoord = snek.Body[0];
					for (auto dir : dirs)
					{
						finalCoord = Shift(finalCoord, dir);
					}
					auto it = std::min_element(board.Treats.begin(), board.Treats.end(),
						[&](Treat t1, Treat t2) { return t1.Coord.ManhattanDist(finalCoord) < t2.Coord.ManhattanDist(finalCoord); });

					penalty = it->Coord.ManhattanDist(finalCoord);
				}

				bool operator <(const Node& second) const
				{
					return penalty < second.penalty;
				}

				Node(const Node& other) = default;
				Node(Node&& other) noexcept = default;
				Node& operator=(const Node& other) = default;
				Node& operator=(Node&& other) noexcept = default;

				/*static bool Compare(const Node& first, const Node& second)
				{
					return first.penalty < second.penalty;
				}*/
			};

			std::vector<Dir> Greedy(const Snek& snek)
			{
				std::priority_queue< std::shared_ptr<Node> > openList;
				openList.emplace( std::make_shared<Node>(snek, board, nullptr, std::vector<Dir>()) );

				std::shared_ptr<Node> best = openList.top();

				int numSteps = 150;
				while( numSteps-- > 0 && !openList.empty() )
				{
					auto current = openList.top();
					openList.pop();

					if (best->penalty > current->penalty)
						best = current;

					auto dirs = StepDirections(snek, current->dirs);
					for (Dir dir : {Left, Right, Up, Down})
					{
						if(dirs.Has(dir))
						{
							std::vector<Dir> newDirs(current->dirs);
							newDirs.emplace_back(dir);
							openList.emplace( std::make_shared<Node>(snek, board, &*current, newDirs) );
						}
					}
				}

				return best->dirs;
			}
		};

		

		void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) override
		{
			MyBoard myBoard(board);

			auto pred = myBoard.Greedy(snek);

			/*Dirs possible = myBoard.StepDirections(snek, {});

			Coord head = snek.Body[0];
			auto it = std::min_element(board.Treats.begin(), board.Treats.end(),
				[&](Treat t1, Treat t2) { return t1.Coord.ManhattanDist(head) < t2.Coord.ManhattanDist(head); }); //noice
			Coord favorite = it->Coord;



			if (head.x > favorite.x && possible.Has(Left)) moveRequest = Left;
			else if (head.x < favorite.x && possible.Has(Right)) moveRequest = Right;
			else if (head.y > favorite.y && possible.Has(Up)) moveRequest = Up;
			else if (head.y < favorite.y && possible.Has(Down)) moveRequest = Down;
			else std::cout << "CANT MOVE";

			//DummyStep(board, snek, moveRequest, boost);*/
			if (pred.size() > 0)
				moveRequest = pred[0];
			else
				std::cout << "NO valid move";
		}
};
