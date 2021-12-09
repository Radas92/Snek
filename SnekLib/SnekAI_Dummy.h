#pragma once

#include "SnekGame.h"
#include "DummyAi.h"
#include "SFML/Window.hpp"
#include <cmath>

struct SnekAI_Dummy : public SnekAI
{
		void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) override
		{
				DummyStep(board, snek, moveRequest, boost);
		}

		void ReportStepDuration(sf::Int64 durationMicroseconds)  override
		{
				if (durationMicroseconds > MicrosecondsLimit)
				{
						std::cout << durationMicroseconds << "\n";
				}
		}

};