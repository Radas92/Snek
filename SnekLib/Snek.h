#pragma once

#include "SFML/Graphics.hpp"
#include <vector>
#include <string>
#include <tuple>
#include "Misc.h"
#include "Teams.h"

struct Board;
struct Snek;

struct SnekAI
{
		static const int MicrosecondsLimit = 1000;
		virtual Team GetTeam() { return Team::Dummy; };
		virtual void Step(const Board& board, const Snek& snek, Dir& moveRequest, bool& boost) {}
		virtual void ReportStepDuration(sf::Int64 durationMicroseconds) {}
};

struct Snek
{
		Snek(SnekAI& ai, int id) :AI(ai), ID(id){}

		std::vector<Coord> Body;
		Dir Heading() const { return m_Heading; }
		unsigned ID;
		bool IsDeath() { return m_Death; }

private:
		friend class SnekGame;
		friend struct Board;
		friend struct Status;
		friend struct View;
		friend struct BatchUpdater;
		
		unsigned FrameOfDeath() { return m_FrameOfDeath; }
		void Move(Dir dir, Board& board, int frameNumber);
		void Reset(Coord headPos);
		int LastIndex(Coord c);
		void ReportStepDuration(sf::Int64 durationMicroseconds, int frameNumber, bool mt);
		void Die(int frameNumber);
		
		SnekAI& AI;
		Dir m_Heading = Up;
		bool m_Death = false;
		int m_BoostMoves = 0;
		unsigned m_FrameOfDeath = -1;
		unsigned m_Strikes = 0;

		sf::Texture m_Avatar0;
		sf::Texture m_Avatar1;

		static const int MovesPerTailSegment = 3;
		static const unsigned MinimalLength = 3;
};