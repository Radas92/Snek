#pragma once

#include "SnekGame.h"
#include "SFML/Window.hpp"
#include "DummyAi.h"

struct SnekAI_PNohejl_MMatous : public SnekAI
{
		virtual Team GetTeam() override { return Team::PNohejlMMatous; };

		void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) override
		{
				DummyStep(board, snek, moveRequest, boost);
		}
};
