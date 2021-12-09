#pragma once

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Snek.h"
#include "Board.h"
#include "Misc.h"
#include "SnekGame.h"

inline float TopOffset = 100.0f;

using Vertices = std::vector<sf::Vertex>;
static sf::RenderWindow* Window = nullptr;
static auto DefaultColor = sf::Color(200, 200, 200);

static std::vector<sf::Color> Colors
{
		sf::Color(255,0,0),
		sf::Color(0,255,0),
		sf::Color(255,191,0),
		sf::Color(0,191,255),
		sf::Color(255,0,191),
		sf::Color(191,0,255),
};

inline sf::Vector2f GetBoardSize()
{
		auto sizeu = Window->getSize();
		return sf::Vector2f{ (float)sizeu.x, (float)sizeu.y - TopOffset };
}

inline sf::Vector2f GetCellSize()
{
		sf::Vector2f size = GetBoardSize();
		return sf::Vector2f(size.x / Board::Cols, size.y / Board::Rows);
}

inline Vertices FillBoardLines()
{
		Vertices lines;

		auto sizeu = Window->getSize();
		sf::Vector2f size = GetBoardSize();
		auto cellSize = GetCellSize();

		sf::Color lineColor(155, 155, 155, 255);

		for (unsigned i = 0; i <= Board::Cols; ++i)
		{
				lines.emplace_back(sf::Vector2f(i * cellSize.x, TopOffset), lineColor);
				lines.emplace_back(sf::Vector2f(i * cellSize.x, (float)size.y + TopOffset), lineColor);
		}
		for (unsigned i = 0; i <= Board::Rows; ++i)
		{
				lines.emplace_back(sf::Vector2f(0, i * cellSize.y + TopOffset), lineColor);
				lines.emplace_back(sf::Vector2f((float)size.x, i * cellSize.y + TopOffset), lineColor);
		}

		return lines;
}

inline sf::RectangleShape GetRectangle(const Coord& coord, int colorIndex = -1)
{
		auto cellSize = GetCellSize();

		sf::RectangleShape rectangle;
		rectangle.setSize(cellSize);
		rectangle.setPosition(coord.x * cellSize.x, coord.y * cellSize.y + TopOffset);
		rectangle.setFillColor(colorIndex < 0 ? DefaultColor : Colors[colorIndex % Colors.size()]);
		return rectangle;
}

inline sf::Color GetColor(int index, sf::Uint8 alpha = 255) {
		auto ret = Colors[index % Colors.size()];
		ret.a = alpha;
		return ret;
}

struct Status
{
		Snek* Snek;

		bool operator>(const Status& rhs)
		{
				if (Snek->FrameOfDeath() == rhs.Snek->FrameOfDeath())
				{
						return Snek->Body.size() > rhs.Snek->Body.size();
				}
				else
				{
						return Snek->FrameOfDeath() > rhs.Snek->FrameOfDeath();
				}
		}
};

struct View
{
		std::vector<Status> Statuses;
		sf::Font Font;

		void Init(SnekGame& game)
		{
				game.Init();

				Window = new sf::RenderWindow(sf::VideoMode(1600, 1000), "Snek!", sf::Style::Titlebar | sf::Style::Close);

				for (auto& s : game.GetBoard().Sneks)
				{
						Status status;
						status.Snek = s.get();
						Statuses.push_back(status);

						auto path = [](const std::string& name) { return "thumb/" + name + "_thumb.jpg"; };

						s->m_Avatar0.loadFromFile(path(GetTeamNames(s->AI.GetTeam()).first));
						s->m_Avatar1.loadFromFile(path(GetTeamNames(s->AI.GetTeam()).second));
				}

				Font.loadFromFile("arial.ttf");
		}

		void Update(SnekGame& game)
		{
				sf::Time elapsed_time;
				sf::Clock r;
				sf::Time delta_time = sf::milliseconds(20);
				
				bool initPhase = true;

				while (Window->isOpen())
				{
						elapsed_time += r.restart();
						if (initPhase && elapsed_time.asSeconds() > 2.0f) {
								initPhase = false;
								elapsed_time = sf::Time::Zero;
						}

						while (!initPhase && elapsed_time >= delta_time)
						{
								elapsed_time -= delta_time;
								game.Update();
						}

						sf::Event event;
						while (Window->pollEvent(event))
						{
								if (event.type == sf::Event::Closed)
								{
										Window->close();
								}
						}

						Window->clear(sf::Color(100, 100, 100, 255));

						static auto lines = FillBoardLines();
						Window->draw(lines.data(), lines.size(), sf::Lines);

						for (auto& snek : game.m_Board.Sneks)
						{
								for (auto& pixel : snek->Body)
								{
										Window->draw(GetRectangle(pixel, snek->ID));
 								}
						}

						for (auto& treat : game.m_Board.Treats)
						{
								Window->draw(GetRectangle(treat.Coord, treat.Owner));
						}

						DrawStatusBar(*Window, game);

						Window->display();
				}
		}

		void DrawStatusBar(sf::RenderWindow& w, const SnekGame& game)
		{
				std::sort(Statuses.begin(), Statuses.end(), std::greater<>());

				sf::Vector2f statusSize = { GetBoardSize().x / Statuses.size(), TopOffset };

				for (unsigned i = 0; i < Statuses.size(); ++i)
				{
						sf::Vector2f pos{ i * statusSize.x, 0 };

						sf::RectangleShape rectangle;
						rectangle.setSize(statusSize);
						rectangle.setPosition(i * statusSize.x, 0);
						rectangle.setFillColor(GetColor(Statuses[i].Snek->ID, Statuses[i].Snek->IsDeath() ? 100 : 255));
						w.draw(rectangle);

						sf::Sprite sprite1;
						sf::Sprite sprite2;
						sprite1.setTexture(Statuses[i].Snek->m_Avatar0);
						sprite2.setTexture(Statuses[i].Snek->m_Avatar1);
						sprite1.setPosition({ pos.x + 10, pos.y + 10 });
						sprite2.setPosition({ pos.x + 100, pos.y + 10 });
						Window->draw(sprite1);
						Window->draw(sprite2);

						sf::Text text;
						text.setFont(Font);
						std::string str = "Team" + std::to_string(i) + " Len: " + std::to_string(Statuses[i].Snek->Body.size());
						if (Statuses[i].Snek->IsDeath())
						{
								str += " FOD: " + std::to_string(Statuses[i].Snek->FrameOfDeath());
						}
						text.setString(str);
						text.setCharacterSize(24);
						text.setFillColor(sf::Color::White);
						text.setPosition({ pos.x + 200, pos.y + 40 });
						w.draw(text);

						if (Statuses[i].Snek->IsDeath())
						{
								Vertices lines =
								{
										{pos, sf::Color::Red},
										{pos + statusSize, sf::Color::Red},
										{{pos.x + statusSize.x, pos.y}, sf::Color::Red},
										{{pos.x, pos.y + statusSize.y}, sf::Color::Red},
								};
								Window->draw(lines.data(), lines.size(), sf::Lines);
						}
				}


				sf::Vector2f timeRectSize = { (float)game.m_FrameNumber / game.MaxSteps * GetBoardSize().x, 5 };

				sf::RectangleShape timeRect;
				timeRect.setSize(timeRectSize);
				timeRect.setFillColor(sf::Color::Blue);
				w.draw(timeRect);
		}
};