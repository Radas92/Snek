#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <execution>
#include <atomic>
#include "AIProvider.h"

using Results = std::map<Team, std::atomic<int>>;

struct BatchUpdater
{
		std::vector<Team> Teams;
		int Runs = 50;
		bool PrintInfo = true;

		static void PrintResults(Results& winners)
		{
				std::vector<std::pair<Team, int>> results;
				for (auto& p : winners)
				{
						results.push_back(p);
				}

				std::sort(results.begin(), results.end(), [](auto& a, auto& b) { return a.second > b.second; });

				std::cout << "\n\nTotal Wins:\n";
				for (auto& p : results)
				{
						std::cout << p.second << "\t" << GetTeamName(p.first) << "\n";
				}

				std::cout << "\n\n";
		}

		static Results CombineResults(Results& r1, Results& r2)
		{
				Results res;
				auto fill = [&](auto& r) 
				{
						for (auto& p : r)
						{
								res[p.first] += p.second;
						}
				};
				fill(r1);
				fill(r2);
				return res;
		}

		inline Results Run()
		{
				auto rd = std::random_device{};
				auto rng = std::default_random_engine{ rd() };

				Results winners;
				for (int i = 0; i < Runs; ++i)
				{
						SnekGame TheGame;
						std::shuffle(std::begin(Teams), std::end(Teams), rng);

						for (auto t : Teams)
						{
								TheGame.PushSnekAI(*GetAI(t));
						}
						TheGame.Init();
						RunImpl(TheGame);

						if (PrintInfo)
						{
								std::cout << "===============================\n";
								std::cout << "Game " << i << " of " << Runs << " concluded:\n";
								for (auto& s : TheGame.GetBoard().Sneks)
								{
										std::cout << GetTeamName(s->AI.GetTeam()) << " FrameOfDeath: " << std::to_string(s->FrameOfDeath()) << " Len: " << s->Body.size() << "\n";
								}
								std::cout << "\n";
						}

						++winners[TheGame.GetWinner().GetTeam()];
				}

				return winners;
		}

		inline Results RunMT()
		{
				auto rd = std::random_device{};
				auto rng = std::default_random_engine{ rd() };

				Results winners;
				std::vector<SnekGame> games(Runs);

				for (auto& game : games)
				{
						std::vector<Team> teams = Teams;
						std::shuffle(std::begin(teams), std::end(teams), rng);
						for (auto t : teams)
						{
								game.PushSnekAI(*GetAI(t));
								winners[t] = 0;
						}
						game.Init();
						game.m_runningMT = true;
				}

				std::for_each(std::execution::par_unseq, games.begin(), games.end(), [&](auto& game) {
						RunImpl(game);
						++winners[game.GetWinner().GetTeam()];
						});

				return winners;

		}

		static void FindBestOf(std::vector<Team> teams, int teamsInGame)
		{
				auto rd = std::random_device{};
				auto rng = std::default_random_engine{ rd() };

				Results results;

				while (true)
				{
						for (int x = 0; x < 100; ++x)
						{
								std::vector<Team> shuffled = teams;
								std::shuffle(std::begin(shuffled), std::end(shuffled), rng);
								shuffled.resize(teamsInGame);

								BatchUpdater batchUpdater;
								batchUpdater.Teams = shuffled;
								batchUpdater.Runs = 100;
								batchUpdater.PrintInfo = false;

								auto res = batchUpdater.RunMT();
								results = BatchUpdater::CombineResults(results, res);

						}

						BatchUpdater::PrintResults(results);
				}
		}

private:
		void RunImpl(SnekGame& game) {
				while (true)
				{
						bool update = game.Update();
						if (!update || game.SneksAlive() <= 1)
						{
								break;
						}
				};
		}
};