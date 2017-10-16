#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <sstream>
#include "stdafx.h"

class Game
{
public:
	Game();

	void Update(sf::RenderWindow* window, float dt);
	void Draw(sf::RenderWindow* window);
	void updateScore(int s);
	void updateTime(int t, float dt);
	sf::SoundBuffer ShatterBuffer;
	sf::Sound ShatterSound;

private:
	int score;
	float time;
	sf::Font m_mainFont;


};
