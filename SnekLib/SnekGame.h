#pragma once

#include <vector>

#include "Board.h"
#include "Misc.h"

class SnekGame
{
public:
		void SetTeams(const std::vector<Team>& teams);
		void PushSnekAI(SnekAI& snek); //takes snekai ownership
		Board& GetBoard() { return m_Board; }
		~SnekGame();

		static const int MaxSteps = 2000;
		unsigned SneksAlive();
		SnekAI& GetWinner();

private:
		void Init();
		bool Update();

		Board m_Board;
		std::vector<SnekAI*> m_AIs;
		int m_FrameNumber;
		bool m_runningMT = false;

		friend struct View;
		friend struct BatchUpdater;
};