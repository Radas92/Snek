#pragma once

#include "SnekGame.h"

#include "SFML/Window.hpp"

struct SnekAI_Human : public SnekAI
{
		virtual Team GetTeam() override { return Team::Human; };

		void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) override
		{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
						moveRequest = Left;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
						moveRequest = Right;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
						moveRequest = Up;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
						moveRequest = Down;
				}
				boost = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		}

};