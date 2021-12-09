#include "SnekGame.h"
#include "View.h"
#include <array>
#include <random>
#include "Snek.h"
#include "AIProvider.h"

void SnekGame::SetTeams(const std::vector<Team>& teams)
{
		for (auto& t : teams)
		{
				PushSnekAI(*GetAI(t));
		}
}

void SnekGame::PushSnekAI(SnekAI& ai)
{
		m_Board.Sneks.emplace_back(std::make_unique<Snek>(ai, m_Board.Sneks.size()));
}

SnekGame::~SnekGame()
{
		for (auto& snek : m_Board.Sneks)
		{
				delete &snek->AI;
		}
}

unsigned SnekGame::SneksAlive()
{
		return std::count_if(m_Board.Sneks.begin(), m_Board.Sneks.end(), [](auto& s) {return !s->IsDeath(); });
}

SnekAI& SnekGame::GetWinner()
{
		auto it = std::max_element(m_Board.Sneks.begin(), m_Board.Sneks.end(), [](const auto& s1, const auto& s2) {
				if (s1->m_FrameOfDeath == s2->m_FrameOfDeath) {
						return s1->Body.size() > s2->Body.size();
				}
				return s1->m_FrameOfDeath < s2->m_FrameOfDeath;
		});
		return (*it)->AI;
}

void SnekGame::Init()
{
		srand(std::random_device()());
		m_Board.Init();
		m_FrameNumber = 0;
}


bool SnekGame::Update()
{
		++m_FrameNumber;

		if (m_FrameNumber > MaxSteps)
		{
				return false;
		}

		m_Board.Update();

		for (auto& snek : m_Board.Sneks)
		{
				if (snek->m_Death)
				{
						continue;
				}

				Dir d = None;
				bool boost = false;

				sf::Clock clock;
				snek->AI.Step(m_Board, *snek, d, boost);
				auto duration = clock.getElapsedTime().asMicroseconds();
				snek->ReportStepDuration(duration, m_FrameNumber, m_runningMT);

				if (d == None)
				{
						d = snek->m_Heading;
				}

				snek->m_Heading = d;
				snek->Move(d, m_Board, m_FrameNumber);

				if (!snek->m_Death && snek->Body.size() > snek->MinimalLength && boost)
				{
						snek->Move(d, m_Board, m_FrameNumber);
						++snek->m_BoostMoves;
						if (snek->m_BoostMoves > Snek::MovesPerTailSegment) {
								snek->m_BoostMoves = 0;
								snek->Body.pop_back();
						}
				}
		}

		for (unsigned s = 0; s < m_Board.Sneks.size(); ++s)
		{
				auto& snek = m_Board.Sneks[s];
				if (snek->m_Death && !snek->Body.empty())
				{
						for (unsigned i = 2; i < snek->Body.size(); i += 3)
						{
								Treat t;
								t.Coord = snek->Body[i];
								t.Owner = s;
								m_Board.Treats.insert(t);
						}

						snek->Body.clear();
				}
		}

		return true;
}

