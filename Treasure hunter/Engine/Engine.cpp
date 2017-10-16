
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>



int ts = 54; //board size
sf::Vector2i offset(48, 24);
int score = 0;
int match = 0;
float timer = 99;

//sf::SoundBuffer ShatterBuffer;
//sf::Sound ShatterSound;


struct tiles
{
	int x, y, col, row, kind, match, alpha;
	tiles() { match = 0; alpha = 255; }
} board[10][10];

void swap(tiles p1, tiles p2)
{
	std::swap(p1.col, p2.col);
	std::swap(p1.row, p2.row);

	board[p1.row][p1.col] = p1;
	board[p2.row][p2.col] = p2;

}


void UpdateTime(float dt)
{
	timer -= dt;
}

int main()
{
	sf::Clock clock;

	sf::Time dt = clock.restart();
	
	sf::Font m_mainFont;
	m_mainFont.loadFromFile("Fonts/Pirate.ttf");

	srand(static_cast<unsigned int>(time(0)));



	sf::RenderWindow Game(sf::VideoMode(740, 480), "Treasure Hunter- Match 3 Gems!");
	Game.setFramerateLimit(60);
	


	sf::Texture t1, t2;
	t1.loadFromFile("Sprites/treasurehunterbackground.png");
	t2.loadFromFile("Sprites/gems2.png");

	sf::Sprite background(t1), gems(t2);


	/*ShatterSound.setBuffer(ShatterBuffer);
	if (!ShatterBuffer.loadFromFile("audio/shatter.ogg"))
	{
		std::cout << "error";
	}
	ShatterSound.setVolume(50);*/

	//sf::Music Music;
	//Music.openFromFile("audio/TreasureHunter.ogg");
	////Music.play();
	//Music.setLoop(true);
	//Music.setVolume(20);

	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
		{
			board[i][j].kind = rand() % 7;
			board[i][j].col = j;
			board[i][j].row = i;
			board[i][j].x = j*ts;
			board[i][j].y = i*ts;
		}

	int x0;
	int x;
	int y;
	int click = 0;
	sf::Vector2i pos;
	int y0;
	bool isSwapped = false;
	bool isMoving = false;


	while (Game.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (Game.pollEvent(event))
		{


			if (event.type == sf::Event::Closed)
				Game.close();

			if (event.type == sf::Event::MouseButtonPressed)
				if (event.key.code == sf::Mouse::Left)
				{
					if (!isSwapped && !isMoving)click++;
					pos = sf::Mouse::getPosition(Game) - offset;
				}

		}

		//Mouse Clicks//
		if (click == 1)
		{
			x0 = pos.x / ts + 1;
			y0 = pos.y / ts + 1;
		}
		if (click == 2)
		{
			x = pos.x / ts + 1;
			y = pos.y / ts + 1;
			if (abs(x - x0) + abs(y - y0) == 1)
			{
				swap(board[y0][x0], board[y][x]);
				isSwapped = 1;
				click = 0;
			}
			else click = 1;
		}

		//Match Found
		for (int i = 1; i <= 8; i++)
			for (int j = 1; j <= 8; j++)
			{
				if (board[i][j].kind == board[i + 1][j].kind)
					if (board[i][j].kind == board[i - 1][j].kind)
						for (int n = -1; n <= 1; n++) board[i + n][j].match++;

				if (board[i][j].kind == board[i][j + 1].kind)
					if (board[i][j].kind == board[i][j - 1].kind)
						for (int n = -1; n <= 1; n++) board[i][j + n].match++;


			}

		//Movement Anim//
		isMoving = false;
		for (int i = 1; i <= 8; i++)
			for (int j = 1; j <= 8; j++)
			{
				tiles &p = board[i][j];
				int dx;
				int dy;
				for (int n = 0; n < 4; n++) // 4-speed
				{
					dx = p.x - p.col*ts;
					dy = p.y - p.row*ts;
					if (dx) p.x -= dx / abs(dx);
					if (dy) p.y -= dy / abs(dy);
				}
				if (dy) isMoving = 1;
			}

		//deleting animation
		if (!isMoving)
			for (int i = 1; i <= 8; i++)
				for (int j = 1; j <= 8; j++)
					if (board[i][j].match)
						if (board[i][j].alpha > 10)
						{
							board[i][j].alpha -= 10;
							isMoving = true;
						}

		//Score System
		int matched;

		for (int i = 1; i <= 8; i++)
			for (int j = 1; j <= 8; j++)
			{
				matched = board[i][j].match;
				//ShatterSound.play();
				match += board[i][j].match;

			}
		//ShatterSound.stop();
		score = match / 20;

		sf::Text Score;
		Score.setFont(m_mainFont);
		Score.setString(std::to_string(score));
		Score.setCharacterSize(50);
		Score.setColor(sf::Color::White);
		Score.setPosition(sf::Vector2f(585, 350));

		//Timer

		sf::Text Timer;
		Timer.setFont(m_mainFont);
		Timer.setColor(sf::Color::Black);
		
		Timer.setString("ENDLESS MODE");
		Timer.setCharacterSize(40);
		Timer.setPosition(sf::Vector2f(496,190));

		sf::Text Timer2;
		Timer2.setFont(m_mainFont);
		Timer2.setColor(sf::Color::White);

		Timer2.setString("ENDLESS MODE");
		Timer2.setCharacterSize(40);
		Timer2.setPosition(sf::Vector2f(497, 190));

		sf::Text TimerNo;
		TimerNo.setFont(m_mainFont);
		TimerNo.setColor(sf::Color::Black);
		TimerNo.setString(std::to_string((int)timer));
		TimerNo.setCharacterSize(50);
		TimerNo.setPosition(sf::Vector2f(525, 275));



		//Second Swap if not matching
		if (isSwapped && !isMoving)
		{
			if (!matched) swap(board[y0][x0], board[y][x]); isSwapped = 0;
		}

		//Updating GameBoard
		if (!isMoving)
		{
			for (int i = 8; i>0; i--)
				for (int j = 1; j <= 8; j++)
					if (board[i][j].match)
						for (int n = i; n>0; n--)
							if (!board[n][j].match)
							{
								swap(board[n][j], board[i][j]); break;
							}

			for (int j = 1; j <= 8; j++)
				for (int i = 8, n = 0; i>0; i--)
					if (board[i][j].match)
					{
						board[i][j].kind = rand() % 7;
						board[i][j].y = -ts*n++;
						board[i][j].match = 0;
						board[i][j].alpha = 255;
					}
		}

		//Draw to screen//


		for (int i = 1; i <= 8; i++)
			for (int j = 1; j <= 8; j++)
			{
				tiles p = board[i][j];
				gems.setTextureRect(sf::IntRect(p.kind * 49, 0, 49, 49));
				gems.setColor(sf::Color(255, 255, 255, p.alpha));
				gems.setPosition(p.x, p.y);
				gems.move(offset.x - ts, offset.y - ts);
				Game.draw(gems);

			}
		Game.display();

		Game.draw(background);
		Game.draw(Score);
		Game.draw(Timer2);
		Game.draw(Timer);
		
		
	/*	Game.draw(TimerNo);*/
		
		

	}

	return 0;
}


