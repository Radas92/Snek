#pragma once

#include "SnekGame.h"
#include "SFML/Window.hpp"
#include "DummyAi.h"

struct SnekAI_JFormanek_VPetrov : public SnekAI
{
		virtual Team GetTeam() override { return Team::JFormanekVPetrov; };

		void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) override
		{
				DummyStep(board, snek, moveRequest, boost);
		}
};
