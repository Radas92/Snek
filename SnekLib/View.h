#pragma once

#include <vector>
#include "SFML/Graphics.hpp"


static sf::RenderWindow* Window = nullptr;

struct View
{
		sf::Font Font;

		void Init()
		{
				Window = new sf::RenderWindow(sf::VideoMode(1600, 1000), "Snek!", sf::Style::Titlebar | sf::Style::Close);
				Font.loadFromFile("arial.ttf");
		}

		void Update()
		{
				sf::CircleShape shape(100.f);
				shape.setFillColor(sf::Color::Green);

				while (Window->isOpen())
				{
						sf::Event event;
						while (Window->pollEvent(event))
						{
								if (event.type == sf::Event::Closed)
								{
										Window->close();
								}
						}


						Window->clear(sf::Color(100, 100, 100, 255));
						Window->draw(shape);

						sf::Text text;
						text.setFont(Font);
						text.setString("Test");
						Window->draw(text);


						Window->display();
				}
		}
};