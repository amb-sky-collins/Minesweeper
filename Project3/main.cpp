#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
using namespace std;

// Tile class
class Tile
{
private:
	bool hidden = true;
	bool mine;
	bool flag;
	int x = 0;
	int y = 0;
	int neighborMines = 0;

public:
	Tile()
	{
		this->mine = false;
		this->flag = false;
		this->x = 0;
		this->y = 0;
	}
	Tile(bool mine, bool flag, int x, int y)
	{
		this->mine = mine;
		this->flag = flag;
		this->x = x;
		this->y = y;
	}
	bool getMine()
	{
		return this->mine;
	}
	bool getFlag()
	{
		return this->flag;
	}
	int getPosX()
	{
		return this->x;
	}
	int getPosY()
	{
		return this->y;
	}
	void setMine()
	{
		this->mine = true;
	}
	void removeMine()
	{
		this->mine = false;
	}
	void setFlag()
	{
		this->flag = true;
	}
	void removeFlag()
	{
		this->flag = false;
	}
	bool getHidden()
	{
		return this->hidden;
	}
	void setHidden()
	{
		this->hidden = true;
	}
	void removeHidden()
	{
		this->hidden = false;
	}
	void setNeighborMines(int num)
	{
		this->neighborMines = num;
	}
	int getNeighborMines()
	{
		return this->neighborMines;
	}
};

// Set text function
void setText(sf::Text& text, float x, float y)
{
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin((textRect.left + textRect.width / 2.0f), (textRect.top + textRect.height / 2.0f));
	text.setPosition(sf::Vector2f(x, y));
}

// Checks if a specific random number was already selected
bool randomNumMatches(vector<int> nums, int randomNum)
{
	for (int i = 0; i < nums.size(); i++)
	{
		if (nums.at(i) == randomNum)
		{
			return true;
		}
	}
	return false;
}

// Get neighbors function
vector<Tile> getNeighbors(Tile tile, vector<Tile> tiles)
{
	vector<Tile> neighbors(0);

	for (int j = 0; j < tiles.size(); j++)
	{
		// Neighbor 1
		if (((tile.getPosX() - 32) == (tiles.at(j).getPosX())) && ((tile.getPosY() - 32) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 2
		else if (((tile.getPosX() - 0) == (tiles.at(j).getPosX())) && ((tile.getPosY() - 32) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 3
		else if (((tile.getPosX() + 32) == (tiles.at(j).getPosX())) && ((tile.getPosY() - 32) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 4
		else if (((tile.getPosX() - 32) == (tiles.at(j).getPosX())) && ((tile.getPosY() - 0) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 5
		else if (((tile.getPosX() + 32) == (tiles.at(j).getPosX())) && ((tile.getPosY() - 0) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 6
		else if (((tile.getPosX() - 32) == (tiles.at(j).getPosX())) && ((tile.getPosY() + 32) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 7
		else if (((tile.getPosX() - 0) == (tiles.at(j).getPosX())) && ((tile.getPosY() + 32) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
		// Neighbor 8
		else if (((tile.getPosX() + 32) == (tiles.at(j).getPosX())) && ((tile.getPosY() + 32) == (tiles.at(j).getPosY())))
		{
			neighbors.push_back(tiles.at(j));
		}
	}

	return neighbors;
}

// Check neighbors function
void checkNeighbors(vector<Tile>* tiles, Tile n)
{
	for (int i = 0; i < (*tiles).size(); i++)
	{
		if (((*tiles).at(i).getPosX() == n.getPosX()) && ((*tiles).at(i).getPosY() == n.getPosY()))
		{
			(*tiles).at(i).removeHidden();
		}
	}
	
	vector<Tile> neighbors = getNeighbors(n, (*tiles));

	int mineCounter = 0;
	for (int i = 0; i < neighbors.size(); i++)
	{
		if (neighbors.at(i).getMine() == true)
		{
			mineCounter += 1;
		}
	}

	if (mineCounter > 0)
	{
		for (int i = 0; i < (*tiles).size(); i++)
		{
			if (((*tiles).at(i).getPosX() == n.getPosX()) && ((*tiles).at(i).getPosY() == n.getPosY()))
			{
				(*tiles).at(i).setNeighborMines(mineCounter);
			}
		}
	}
	// If neighbor is not a mine
	else if (mineCounter == 0)
	{
		for (int j = 0; j < neighbors.size(); j++)
		{
			if ((neighbors.at(j).getHidden() == true) && (neighbors.at(j).getFlag() == false))
			{
				checkNeighbors(tiles, neighbors.at(j));
			}
		}
	}
}

// Main function
int main()
{
	// Variables
	ifstream inFile;
	string playerName;
	int numCols = 0;
	int numRows = 0;
	int numMines = 0;
	int width = 0;
	int height = 0;
	int lWidth = 0;
	int lHeight = 0;
	int numTiles = 0;
	int numFlags = 0;
	bool moveToGame = false;
	vector<string>strings(0);
	vector<string> times(0);
	vector<string> players(0);
	vector<Tile> tiles(0);
	string leaderboard;
	sf::Text titleText;

	// Opens file
	inFile.open("files/board_config.cfg");
	// Checks if file is open
	if (inFile.is_open())
	{
		inFile >> numCols;
		inFile >> numRows;
		inFile >> numMines;
	}
	// Closes file
	inFile.close();

	// Sets variables for width, height, and numTiles
	width = numCols * 32;
	height = (numRows * 32) + 100;
	lWidth = 16 * numCols;
	lHeight = (16 * numRows) + 50;
	numTiles = numCols * numRows;

	// Sets text font
	sf::Font font;
	font.loadFromFile("files/font.ttf");

	// Sets welcome text
	sf::Text welcomeText;
	welcomeText.setFont(font);
	welcomeText.setString("WELCOME TO MINESWEEPER!");
	welcomeText.setCharacterSize(24);
	welcomeText.setFillColor(sf::Color::White);
	welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	sf::FloatRect textRect = welcomeText.getLocalBounds();
	welcomeText.setOrigin((textRect.width / 2.0f), 0);
	welcomeText.setPosition((width / 2.0f), ((height / 2.0f) - 150));

	// Sets input prompt text
	sf::Text inPromptText;
	inPromptText.setFont(font);
	inPromptText.setString("Enter your name:");
	inPromptText.setCharacterSize(20);
	inPromptText.setFillColor(sf::Color::White);
	inPromptText.setStyle(sf::Text::Bold);
	textRect = inPromptText.getLocalBounds();
	inPromptText.setOrigin((textRect.width / 2.0f), 0);
	inPromptText.setPosition((width / 2.0f), ((height / 2.0f) - 75));

	// Sets input text
	sf::Text inText;
	inText.setFont(font);
	inText.setString("|");
	inText.setCharacterSize(18);
	inText.setFillColor(sf::Color::Yellow);
	inText.setStyle(sf::Text::Bold);
	textRect = inText.getLocalBounds();
	inText.setOrigin((textRect.width / 2.0f), 0);
	inText.setPosition((width / 2.0f), ((height / 2.0f) - 45));

	// Renders welcome window
	sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
	while (window.isOpen())
	{
		// Catches events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Closes window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				// Backspaces name
				if (event.key.code == sf::Keyboard::Backspace)
				{
					string text = inText.getString();
					
					if (text.size() > 1)
					{
						string subStr = text.substr(0, (text.size() - 2));
						string newText = subStr + "|";

						inText.setString(newText);
						textRect = inText.getLocalBounds();
						inText.setOrigin((textRect.width / 2.0f), 0);
					}
				}
				// Pressing enter moves to game window 
				else if (event.key.code == sf::Keyboard::Enter)
				{
					// Ensures name is typed
					if ((inText.getString()).getSize() > 1)
					{
						moveToGame = true;
						window.close();
					}
				}
			}
			// Inputs name
			else if (event.type == sf::Event::TextEntered)
			{
				// Ensures only letters can be typed
				if (((event.text.unicode >= 97) && (event.text.unicode <= 122)) || ((event.text.unicode >= 65) && (event.text.unicode <= 90)))
				{
					char ch = char(event.text.unicode);
					string text = inText.getString();
					string subStr = text.substr(0, (text.size() - 1));
					string newText;

					// Ensures that name is no more than 10 characters
					if (text.size() < 11)
					{
						// Capitalizes first letter in name
						if (text.size() == 1)
						{
							ch = toupper(ch);
							newText = ch + text;
						}
						// Makes other letters lowercase
						else
						{
							ch = tolower(ch);
							newText = subStr + ch + "|";
						}

						playerName = newText.substr(0, newText.size() - 1);
						inText.setString(newText);
						textRect = inText.getLocalBounds();
						inText.setOrigin((textRect.width / 2.0f), 0);
					}
				}
			}
		}

		window.clear(sf::Color::Blue);
		window.draw(welcomeText);
		window.draw(inPromptText);
		window.draw(inText);
		window.display();
	}

	// Renders game window
	sf::RenderWindow gWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
	if (moveToGame)
	{
		bool debug = false;

		// Sets textures
		sf::Texture faceTexture;

		sf::Texture debugTexture;
		debugTexture.loadFromFile("files/images/debug.png");

		sf::Texture pTexture;

		sf::Texture leaderboardTexture;
		leaderboardTexture.loadFromFile("files/images/leaderboard.png");

		sf::Texture tileTexture;
		tileTexture.loadFromFile("files/images/tile_hidden.png");

		sf::Texture flagTexture;
		sf::Texture mineTexture;
		sf::Texture numTileTexture;

		sf::Texture mineCounter1Texture;
		sf::Texture mineCounter2Texture;
		sf::Texture mineCounter3Texture;
		sf::Texture minusTexture;

		sf::Texture timer1Texture;
		sf::Texture timer2Texture;
		sf::Texture timer3Texture;
		sf::Texture timer4Texture;

		// Sets sprites
		sf::Sprite mineCounter1;
		sf::Sprite mineCounter2;
		sf::Sprite mineCounter3;
		sf::Sprite minus;

		sf::Sprite timer1;
		sf::Sprite timer2;
		sf::Sprite timer3;
		sf::Sprite timer4;

		// Sets position of all tile sprites
		for (int i = 0; i < numCols; i++)
		{
			for (int j = 0; j < numRows; j++)
			{
				Tile tile = Tile(false, false, (i * 32), (j * 32));
				tiles.push_back(tile);
			}
		}

		vector<int> randomNums(0);
		// Makes random tiles mines
		for (int i = 0; i < numMines; i++)
		{
			bool numMatches = false;

			// Generates random number
			int randomNum = rand() % numTiles;

			// Sets mines
			if (randomNums.size() == 0)
			{
				tiles.at(randomNum).setMine();
				randomNums.push_back(randomNum);
			}
			// Checks if a random number has already been used
			else
			{
				numMatches = randomNumMatches(randomNums, randomNum);
				while (numMatches == true)
				{
					randomNum = rand() % numTiles;
					numMatches = randomNumMatches(randomNums, randomNum);
				}
				tiles.at(randomNum).setMine();
				randomNums.push_back(randomNum);
			}
		}

		// Sets timer
		timer4Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
		timer4.setTexture(timer4Texture);
		timer4.setPosition((numCols * 32) - 97, (32 * (numRows + 0.5f)) + 16);

		timer3Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
		timer3.setTexture(timer3Texture);
		timer3.setPosition((numCols * 32) - 76, (32 * (numRows + 0.5f)) + 16);

		timer2Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
		timer2.setTexture(timer2Texture);
		timer2.setPosition((numCols * 32) - 54, (32 * (numRows + 0.5f)) + 16);

		timer1Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
		timer1.setTexture(timer1Texture);
		timer1.setPosition((numCols * 32) - 33, (32 * (numRows + 0.5f)) + 16);

		auto time = chrono::high_resolution_clock::now();
		chrono::seconds eTime;
		chrono::high_resolution_clock::time_point pausedTime1;
		chrono::high_resolution_clock::time_point pausedTime2;
		chrono::seconds ePausedTime = chrono::seconds(0);
		
		// Game window
		while (gWindow.isOpen())
		{
			bool leaderboardOpen = false;

			// Set buttons
			faceTexture.loadFromFile("files/images/face_happy.png");
			sf::Sprite faceSprite;
			faceSprite.setTexture(faceTexture);
			faceSprite.setPosition((((numCols / 2.0) * 32) - 32), (32 * (numRows + 0.5f)));

			sf::Sprite debugSprite;
			debugSprite.setTexture(debugTexture);
			debugSprite.setPosition((width - 304), (32 * (numRows + 0.5f)));

			pTexture.loadFromFile("files/images/pause.png");
			sf::Sprite pSprite;
			pSprite.setTexture(pTexture);
			pSprite.setPosition((width - 240), (32 * (numRows + 0.5f)));

			sf::Sprite leaderboardSprite;
			leaderboardSprite.setTexture(leaderboardTexture);
			leaderboardSprite.setPosition((width - 176), (32 * (numRows + 0.5f)));

			// Updates timer
			auto time2 = chrono::high_resolution_clock::now();
			if (ePausedTime == chrono::seconds(0))
			{
				eTime = chrono::duration_cast<chrono::seconds>(time2 - time);
			}
			else
			{
				eTime = chrono::duration_cast<chrono::seconds>(time2 - time);
				eTime = eTime - ePausedTime;
			}
			
			int minutes = 0;
			int seconds = 0;
			if (eTime.count() >= 60)
			{
				minutes = eTime.count() / 60;
				seconds = eTime.count() % 60;
			}
			else
			{
				seconds = eTime.count();
			}

			string strMinutes = to_string(minutes);
			string strSeconds = to_string(seconds);
			if (strMinutes.size() == 1)
			{
				timer4Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
				timer4.setTexture(timer4Texture);
				timer4.setPosition((numCols * 32) - 97, (32 * (numRows + 0.5f)) + 16);

				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strMinutes[0] == str[0])
					{
						timer3Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						timer3.setTexture(timer3Texture);
						timer3.setPosition((numCols * 32) - 76, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}
			else if (strMinutes.size() == 2)
			{
				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strMinutes[1] == str[0])
					{
						timer3Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						timer3.setTexture(timer3Texture);
						timer3.setPosition((numCols * 32) - 76, (32 * (numRows + 0.5f)) + 16);
					}
					if (strMinutes[0] == str[0])
					{
						timer4Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						timer4.setTexture(timer4Texture);
						timer4.setPosition((numCols * 32) - 97, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}
			if (strSeconds.size() == 1)
			{
				timer2Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
				timer2.setTexture(timer2Texture);
				timer2.setPosition((numCols * 32) - 54, (32 * (numRows + 0.5f)) + 16);

				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strSeconds[0] == str[0])
					{
						timer1Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						timer1.setTexture(timer1Texture);
						timer1.setPosition((numCols * 32) - 33, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}
			else if (strSeconds.size() == 2)
			{
				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strSeconds[1] == str[0])
					{
						timer1Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						timer1.setTexture(timer1Texture);
						timer1.setPosition((numCols * 32) - 33, (32 * (numRows + 0.5f)) + 16);
					}
					if (strSeconds[0] == str[0])
					{
						timer2Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						timer2.setTexture(timer2Texture);
						timer2.setPosition((numCols * 32) - 54, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}

			// Sets mine counter
			string strMines = to_string(numMines - numFlags);
			bool negative = false;
			if (strMines.substr(0, 1) == "-")
			{
				strMines = strMines.substr(1);

				minusTexture.loadFromFile("files/images/digits.png", sf::IntRect(210, 0, 21, 32));
				minus.setTexture(minusTexture);
				minus.setPosition(12, (32 * (numRows + 0.5f)) + 16);

				negative = true;
			}
			if (strMines.size() == 3)
			{
				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strMines[0] == str[0])
					{
						mineCounter3Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						mineCounter3.setTexture(mineCounter3Texture);
						mineCounter3.setPosition(33, (32 * (numRows + 0.5f)) + 16);
					}
					if (strMines[1] == str[0])
					{
						mineCounter2Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						mineCounter2.setTexture(mineCounter2Texture);
						mineCounter2.setPosition(54, (32 * (numRows + 0.5f)) + 16);
					}
					if (strMines[2] == str[0])
					{
						mineCounter1Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						mineCounter1.setTexture(mineCounter1Texture);
						mineCounter1.setPosition(75, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}
			else if (strMines.size() == 2)
			{
				mineCounter3Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
				mineCounter3.setTexture(mineCounter3Texture);
				mineCounter3.setPosition(33, (32 * (numRows + 0.5f)) + 16);

				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strMines[0] == str[0])
					{
						mineCounter2Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						mineCounter2.setTexture(mineCounter2Texture);
						mineCounter2.setPosition(54, (32 * (numRows + 0.5f)) + 16);
					}
					if (strMines[1] == str[0])
					{
						mineCounter1Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						mineCounter1.setTexture(mineCounter1Texture);
						mineCounter1.setPosition(75, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}
			else if (strMines.size() == 1)
			{
				mineCounter3Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
				mineCounter3.setTexture(mineCounter3Texture);
				mineCounter3.setPosition(33, (32 * (numRows + 0.5f)) + 16);

				mineCounter2Texture.loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));
				mineCounter2.setTexture(mineCounter2Texture);
				mineCounter2.setPosition(54, (32 * (numRows + 0.5f)) + 16);

				for (int i = 0; i <= 9; i++)
				{
					string str = to_string(i);
					if (strMines[0] == str[0])
					{
						mineCounter1Texture.loadFromFile("files/images/digits.png", sf::IntRect((21 * i), 0, 21, 32));
						mineCounter1.setTexture(mineCounter1Texture);
						mineCounter1.setPosition(75, (32 * (numRows + 0.5f)) + 16);
					}
				}
			}

			// Determines victory
			bool victory = true;
			for (int i = 0; i < tiles.size(); i++)
			{
				if (tiles.at(i).getMine() == true)
				{
					if (tiles.at(i).getFlag() == false)
					{
						victory = false;
					}
				}
				else if (tiles.at(i).getMine() == false)
				{
					if (tiles.at(i).getHidden() == true)
					{
						victory = false;
					}
				}
			}

			// Catches events
			sf::Event event;
			while (gWindow.pollEvent(event))
			{
				// Closes window
				if (event.type == sf::Event::Closed)
				{
					gWindow.close();
				}
				else if (victory == true)
				{
					int placement = -1;

					// Makes face the win face
					faceTexture.loadFromFile("files/images/face_win.png");
					faceSprite.setTexture(faceTexture);
					faceSprite.setPosition((((numCols / 2.0) * 32) - 32), (32 * (numRows + 0.5f)));

					// Displays sprites
					sf::Sprite tileSprite;
					sf::Sprite flagSprite;
					sf::Sprite numTile;
					for (int i = 0; i < tiles.size(); i++)
					{
						if (tiles.at(i).getFlag() == true)
						{
							tileTexture.loadFromFile("files/images/tile_hidden.png");
							tileSprite.setTexture(tileTexture);
							tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

							flagTexture.loadFromFile("files/images/flag.png");
							flagSprite.setTexture(flagTexture);
							flagSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
						}
						else if ((tiles.at(i).getHidden() == false) && (tiles.at(i).getNeighborMines() > 0))
						{
							tileTexture.loadFromFile("files/images/tile_revealed.png");
							tileSprite.setTexture(tileTexture);
							tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

							string strNum = to_string(tiles.at(i).getNeighborMines());
							string file = "files/images/number_" + strNum + ".png";

							numTileTexture.loadFromFile(file);
							numTile.setTexture(numTileTexture);
							numTile.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
						}
						else if ((tiles.at(i).getHidden() == false) && (tiles.at(i).getNeighborMines() == 0))
						{
							tileTexture.loadFromFile("files/images/tile_revealed.png");
							tileSprite.setTexture(tileTexture);
							tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
						}
						gWindow.draw(tileSprite);
						gWindow.draw(numTile);
						gWindow.draw(flagSprite);
					}
					gWindow.draw(faceSprite);
					gWindow.draw(mineCounter1);
					gWindow.display();

					// Until the user clicks restart game
					sf::Event event;
					bool newGame = false;
					bool alreadyClicked = false;
					while (newGame == false)
					{
						if (gWindow.pollEvent(event))
						{
							// If user closes window
							if (event.type == sf::Event::Closed)
							{
								gWindow.close();
								break;
							}
							// If user clicks restart button (face icon)
							else if (((event.mouseButton.x > ((numCols / 2.0) * 32) - 32) && (event.mouseButton.x < (((numCols / 2.0) * 32) - 32) + 64) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64)))))
							{
								for (int i = (randomNums.size() - 1); i >= 0; i--)
								{
									tiles.at(randomNums.at(i)).removeMine();
									randomNums.pop_back();
								}

								for (int i = 0; i < numMines; i++)
								{
									bool numMatches = false;

									// Generates random number
									int randomNum = rand() % numTiles;

									// Sets mines
									if (randomNums.size() == 0)
									{
										tiles.at(randomNum).setMine();
										randomNums.push_back(randomNum);
									}
									// Checks if a random number has already been used
									else
									{
										numMatches = randomNumMatches(randomNums, randomNum);
										while (numMatches == true)
										{
											randomNum = rand() % numTiles;
											numMatches = randomNumMatches(randomNums, randomNum);
										}
										tiles.at(randomNum).setMine();
										randomNums.push_back(randomNum);
									}
								}

								// Resets other info
								for (int i = 0; i < tiles.size(); i++)
								{
									tiles.at(i).setHidden();
									tiles.at(i).removeFlag();
									tiles.at(i).setNeighborMines(0);
									numFlags = 0;
								}

								for (int i = 0; i < players.size(); i++)
								{
									players.pop_back();
									times.pop_back();
									strings.pop_back();
								}

								newGame = true;
								time = chrono::high_resolution_clock::now();
								ePausedTime = chrono::seconds(0);
							}
							// If user clicks leaderboard button
							else if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
							{
								// Variables
								string str;
								leaderboardOpen = true;
								string finishTime = "";
								leaderboard = "";

								// Sets title text
								titleText.setFont(font);
								titleText.setString("LEADERBOARD");
								titleText.setCharacterSize(20);
								titleText.setFillColor(sf::Color::White);
								titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
								sf::FloatRect textRect = titleText.getLocalBounds();
								titleText.setOrigin((textRect.width / 2.0f), 0);
								titleText.setPosition((lWidth / 2.0f), ((lHeight / 2.0f) - 120));

								// Updates leaderboard with new info
								if ((players.size() != 0) && (alreadyClicked == false))
								{
									int size = players.size();
									for (int i = 0; i < size; i++)
									{
										players.pop_back();
										times.pop_back();
										strings.pop_back();
									}
								}
		
								if ((players.size() == 0) && (alreadyClicked == false))
								{
									// Opens file
									inFile.open("files/leaderboard.txt");
									// Checks if file is open
									if (inFile.is_open())
									{
										// Gets ranking text
										while (!inFile.eof())
										{
											inFile >> str;
											strings.push_back(str);
										}
										
										// Sets times vector
										for (int i = 0; i < strings.size(); i++)
										{
											int index = strings.at(i).find(",");
											string time = strings.at(i).substr(0, index);
											times.push_back(time);
										}

										// Sets players vector
										for (int i = 0; i < strings.size(); i++)
										{
											int index = strings.at(i).find(",");
											string player = strings.at(i).substr(index + 1, strings.at(i).size() - 1);
											players.push_back(player);
										}
									}
									// Closes file
									inFile.close();

									if (strMinutes.size() == 1)
									{
										strMinutes = '0' + strMinutes;
									}
									if (strSeconds.size() == 1)
									{
										strSeconds = '0' + strSeconds;
									}
									finishTime = strMinutes + ':' + strSeconds;
									string finishTimeStr = strMinutes + '.' + strSeconds;
									float finishTimeFloat = stof(finishTimeStr);

									vector<float> timesFloat(0);
									for (int i = 0; i < times.size(); i++)
									{
										string timeFloat = "";
										string timeFloat1 = "";
										string timeFloat2 = "";
										if (times.at(i).substr(0, 1) == "0")
										{
											timeFloat1 = times.at(i).substr(1, 1);
											timeFloat2 = times.at(i).substr(3, 5);
											timeFloat = timeFloat1 + '.' + timeFloat2;
											timesFloat.push_back(stof(timeFloat));
										}
										else
										{
											timeFloat1 = times.at(i).substr(0, 2);
											timeFloat2 = times.at(i).substr(3, 5);
											timeFloat = timeFloat1 + '.' + timeFloat2;
											timesFloat.push_back(stof(timeFloat));
										}
									}

									// Determines placement (if any) of new score
									for (int i = 0; i < timesFloat.size(); i++)
									{
										if (finishTimeFloat < timesFloat.at(i))
										{
											placement = i;
											break;
										}
									}

									auto iterPlayers = players.begin();
									auto iterTimes = times.begin();
									auto iterStr = strings.begin();
									if (placement != -1)
									{
										players.emplace(iterPlayers + placement, playerName);
										times.emplace(iterTimes + placement, finishTime);
										players.pop_back();
										times.pop_back();

										string newStr = finishTime + ',' + playerName;
										strings.emplace(iterStr + placement, newStr);
										strings.pop_back();
									}

									// Writes leaderboard info
									if (alreadyClicked == false)
									{
										ofstream outFile;
										outFile.open("files/leaderboard.txt");

										for (int i = 0; i < strings.size(); i++)
										{
											if (i == (strings.size() - 1))
											{
												outFile << strings.at(i);
											}
											else
											{
												outFile << strings.at(i) << endl;
											}
										}

										outFile.close();

										alreadyClicked = true;
									}
								}

								// Sets leaderboard string
								for (int i = 0; i < players.size(); i++)
								{
									if (i == placement)
									{
										leaderboard += to_string(i + 1) + ".\t" + times.at(i) + "\t" + players.at(i) + '*' + "\n" + "\n";
									}
									else
									{
										leaderboard += to_string(i + 1) + ".\t" + times.at(i) + "\t" + players.at(i) + "\n" + "\n";
									}
								}

								// Renders leaderboard window
								sf::RenderWindow lWindow(sf::VideoMode(lWidth, lHeight), "Minesweeper", sf::Style::Close);
								while (lWindow.isOpen())
								{
									// Focuses leaderboard window
									lWindow.requestFocus();

									// Catches events
									sf::Event lEvent;
									while (lWindow.pollEvent(lEvent))
									{
										// Closes leaderboard
										if (lEvent.type == sf::Event::Closed)
										{
											lWindow.close();
										}
									}
									if (gWindow.pollEvent(event))
									{
										// If user clicks leaderboard button
										if (event.type == sf::Event::MouseButtonPressed)
										{
											if (event.mouseButton.button == sf::Mouse::Left)
											{
												if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
												{

												}
											}
											else if (event.type == sf::Event::Closed)
											{

											}
										}
									}

									// Sets leaderboard text
									sf::Text ranking;
									ranking.setFont(font);
									ranking.setString(leaderboard);
									ranking.setCharacterSize(18);
									ranking.setFillColor(sf::Color::White);
									setText(ranking, (lWidth / 2.0f), ((lHeight / 2.0f) + 20));

									// Displays leaderboard
									lWindow.clear(sf::Color::Blue);
									lWindow.draw(titleText);
									lWindow.draw(ranking);
									lWindow.display();
								}
							}
						}
					}
				}
				// If leaderboard is open, do nothing
				else if (leaderboardOpen)
				{
					
				}
				// If user presses mouse button
				else if (event.type == sf::Event::MouseButtonPressed)
				{
					// If user left clicks
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						// If user clicks leaderboard button
						if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
						{	
							pausedTime1 = chrono::high_resolution_clock::now();

							// Displays tiles_revealed
							sf::Sprite tileSprite;
							for (int i = 0; i < tiles.size(); i++)
							{
								tileTexture.loadFromFile("files/images/tile_revealed.png");
								tileSprite.setTexture(tileTexture);
								tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

								gWindow.draw(tileSprite);
							}
							gWindow.display();

							// Variables
							string str;
							leaderboardOpen = true;

							// Sets title text
							titleText.setFont(font);
							titleText.setString("LEADERBOARD");
							titleText.setCharacterSize(20);
							titleText.setFillColor(sf::Color::White);
							titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
							sf::FloatRect textRect = titleText.getLocalBounds();
							titleText.setOrigin((textRect.width / 2.0f), 0);
							titleText.setPosition((lWidth / 2.0f), ((lHeight / 2.0f) - 120));

							// Checks that leaderboard has not been read already
							if (players.size() == 0)
							{
								// Opens file
								inFile.open("files/leaderboard.txt");
								// Checks if file is open
								if (inFile.is_open())
								{
									// Gets ranking text
									while (!inFile.eof())
									{
										inFile >> str;
										strings.push_back(str);
									}

									// Sets times vector
									for (int i = 0; i < strings.size(); i++)
									{
										int index = strings.at(i).find(",");
										string time = strings.at(i).substr(0, index);
										times.push_back(time);
									}

									// Sets players vector
									for (int i = 0; i < strings.size(); i++)
									{
										int index = strings.at(i).find(",");
										string player = strings.at(i).substr(index + 1, strings.at(i).size() - 1);
										players.push_back(player);
									}

									// Sets leaderboard string
									for (int i = 0; i < players.size(); i++)
									{
										leaderboard += to_string(i + 1) + ".\t" + times.at(i) + "\t" + players.at(i) + "\n" + "\n";
									}
								}
								// Closes file
								inFile.close();
							}

							// Renders leaderboard window
							sf::RenderWindow lWindow(sf::VideoMode(lWidth, lHeight), "Minesweeper", sf::Style::Close);
							while (lWindow.isOpen())
							{
								// Focuses leaderboard window
								lWindow.requestFocus();

								// Catches events
								sf::Event lEvent;
								while (lWindow.pollEvent(lEvent))
								{
									// Closes leaderboard
									if (lEvent.type == sf::Event::Closed)
									{
										lWindow.close();

										pausedTime2 = chrono::high_resolution_clock::now();
										ePausedTime += chrono::duration_cast<chrono::seconds>(pausedTime2 - pausedTime1);
									}
								}
								if (gWindow.pollEvent(event))
								{
									if (event.type == sf::Event::Closed)
									{

									}
								}

								// Sets leaderboard text
								sf::Text ranking;
								ranking.setFont(font);
								ranking.setString(leaderboard);
								ranking.setCharacterSize(18);
								ranking.setFillColor(sf::Color::White);
								setText(ranking, (lWidth / 2.0f), ((lHeight / 2.0f) + 20));

								// Displays leaderboard
								lWindow.clear(sf::Color::Blue);
								lWindow.draw(titleText);
								lWindow.draw(ranking);
								lWindow.display();
							}
						}
						// If user clicks debug button
						else if (((event.mouseButton.x > (width - 304)) && (event.mouseButton.x < ((width - 304) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
						{
							if (debug == false)
							{
								debug = true;
							}
							else if (debug == true)
							{
								debug = false;
							}
						}
						// If user clicks restart button (face icon)
						else if (((event.mouseButton.x > ((numCols / 2.0) * 32) - 32) && (event.mouseButton.x < (((numCols / 2.0) * 32) - 32) + 64) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64)))))
						{
							for (int i = (randomNums.size() - 1); i >= 0; i--)
							{
								tiles.at(randomNums.at(i)).removeMine();
								randomNums.pop_back();
							}

							for (int i = 0; i < numMines; i++)
							{
								bool numMatches = false;

								// Generates random number
								int randomNum = rand() % numTiles;

								// Sets mines
								if (randomNums.size() == 0)
								{
									tiles.at(randomNum).setMine();
									randomNums.push_back(randomNum);
								}
								// Checks if a random number has already been used
								else
								{
									numMatches = randomNumMatches(randomNums, randomNum);
									while (numMatches == true)
									{
										randomNum = rand() % numTiles;
										numMatches = randomNumMatches(randomNums, randomNum);
									}
									tiles.at(randomNum).setMine();
									randomNums.push_back(randomNum);
								}

								// Resets other info
								for (int i = 0; i < tiles.size(); i++)
								{
									tiles.at(i).setHidden();
									tiles.at(i).removeFlag();
									tiles.at(i).setNeighborMines(0);
									numFlags = 0;
								}
							}

							time = chrono::high_resolution_clock::now();
							ePausedTime = chrono::seconds(0);
						}
						// If user clicks pause/play button
						else if (((event.mouseButton.x > (width - 240)) && (event.mouseButton.x < ((width - 240) + 64)) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64)))))
						{
							pausedTime1 = chrono::high_resolution_clock::now();
							bool paused = true;

							// Loads play button
							pTexture.loadFromFile("files/images/play.png");
							pSprite.setTexture(pTexture);
							pSprite.setPosition((width - 240), (32 * (numRows + 0.5f)));
							gWindow.draw(pSprite);

							// Displays tiles_revealed
							sf::Sprite tileSprite;
							for (int i = 0; i < tiles.size(); i++)
							{
								tileTexture.loadFromFile("files/images/tile_revealed.png");
								tileSprite.setTexture(tileTexture);
								tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

								gWindow.draw(tileSprite);
							}

							gWindow.display();

							while (paused == true)
							{
								if (gWindow.pollEvent(event))
								{
									// If user closes window
									if (event.type == sf::Event::Closed)
									{
										gWindow.close();
										break;
									}
									// If user clicks play button
									else if (event.type == sf::Event::MouseButtonPressed)
									{
										if (event.mouseButton.button == sf::Mouse::Left)
										{
											if (((event.mouseButton.x > (width - 240)) && (event.mouseButton.x < ((width - 240) + 64)) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64)))))
											{
												paused = false;
												pausedTime2 = chrono::high_resolution_clock::now();
												ePausedTime += chrono::duration_cast<chrono::seconds>(pausedTime2 - pausedTime1);
											}
										}
									}
									// If user clicks leaderboard button
									else if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
									{
										// Variables
										string str;
										leaderboardOpen = true;

										// Sets title text
										titleText.setFont(font);
										titleText.setString("LEADERBOARD");
										titleText.setCharacterSize(20);
										titleText.setFillColor(sf::Color::White);
										titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
										sf::FloatRect textRect = titleText.getLocalBounds();
										titleText.setOrigin((textRect.width / 2.0f), 0);
										titleText.setPosition((lWidth / 2.0f), ((lHeight / 2.0f) - 120));

										// Checks that leaderboard has not been read already
										if (players.size() == 0)
										{
											// Opens file
											inFile.open("files/leaderboard.txt");
											// Checks if file is open
											if (inFile.is_open())
											{
												// Gets ranking text
												while (!inFile.eof())
												{
													inFile >> str;
													strings.push_back(str);
												}

												// Sets times vector
												for (int i = 0; i < strings.size(); i++)
												{
													int index = strings.at(i).find(",");
													string time = strings.at(i).substr(0, index);
													times.push_back(time);
												}

												// Sets players vector
												for (int i = 0; i < strings.size(); i++)
												{
													int index = strings.at(i).find(",");
													string player = strings.at(i).substr(index + 1, strings.at(i).size() - 1);
													players.push_back(player);
												}

												// Sets leaderboard string
												for (int i = 0; i < players.size(); i++)
												{
													leaderboard += to_string(i + 1) + ".\t" + times.at(i) + "\t" + players.at(i) + "\n" + "\n";
												}
											}
											// Closes file
											inFile.close();
										}

										// Renders leaderboard window
										sf::RenderWindow lWindow(sf::VideoMode(lWidth, lHeight), "Minesweeper", sf::Style::Close);
										while (lWindow.isOpen())
										{
											// Focuses leaderboard window
											lWindow.requestFocus();

											// Catches events
											sf::Event lEvent;
											while (lWindow.pollEvent(lEvent))
											{
												// Closes leaderboard
												if (lEvent.type == sf::Event::Closed)
												{
													lWindow.close();
												}
											}
											if (gWindow.pollEvent(event))
											{
												// If user clicks leaderboard button
												if (event.type == sf::Event::MouseButtonPressed)
												{
													if (event.mouseButton.button == sf::Mouse::Left)
													{
														if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
														{

														}
													}
												}
											}

											// Sets leaderboard text
											sf::Text ranking;
											ranking.setFont(font);
											ranking.setString(leaderboard);
											ranking.setCharacterSize(18);
											ranking.setFillColor(sf::Color::White);
											setText(ranking, (lWidth / 2.0f), ((lHeight / 2.0f) + 20));

											// Displays leaderboard
											lWindow.clear(sf::Color::Blue);
											lWindow.draw(titleText);
											lWindow.draw(ranking);
											lWindow.display();
										}
									}
								}
							}
						}
						// If user clicks a tile
						else if ((event.mouseButton.x > 0) && (event.mouseButton.x < (numCols * 32)) && ((event.mouseButton.y > 0) && (event.mouseButton.y < (numRows * 32))))
						{
							bool newGame = false;

							for (int i = 0; i < tiles.size(); i++)
							{
								if (((event.mouseButton.x >= tiles.at(i).getPosX()) && (event.mouseButton.x <= (tiles.at(i).getPosX() + 32))) && ((event.mouseButton.y >= tiles.at(i).getPosY()) && (event.mouseButton.y <= (tiles.at(i).getPosY() + 32))) && (tiles.at(i).getFlag() == false))
								{
									// If user clicks a mine, they lose
									if (tiles.at(i).getMine() == true)
									{
										// Reveals all mines
										for (int i = 0; i < tiles.size(); i++)
										{
											sf::Sprite tileSprite;
											sf::Sprite mineSprite;
											if (tiles.at(i).getMine() == true)
											{
												tileTexture.loadFromFile("files/images/tile_hidden.png");
												tileSprite.setTexture(tileTexture);
												tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

												mineTexture.loadFromFile("files/images/mine.png");
												mineSprite.setTexture(mineTexture);
												mineSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
											}
											gWindow.draw(mineSprite);
										}

										// Makes face the lose face
										faceTexture.loadFromFile("files/images/face_lose.png");
										faceSprite.setTexture(faceTexture);
										faceSprite.setPosition((((numCols / 2.0) * 32) - 32), (32 * (numRows + 0.5f)));

										gWindow.draw(faceSprite);
										gWindow.display();

										// Until the user clicks restart game
										while (newGame == false)
										{
											if (gWindow.pollEvent(event))
											{
												// If user closes window
												if (event.type == sf::Event::Closed)
												{
													gWindow.close();
													break;
												}
												// If user clicks restart button (face icon)
												else if (((event.mouseButton.x > ((numCols / 2.0) * 32) - 32) && (event.mouseButton.x < (((numCols / 2.0) * 32) - 32) + 64) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64)))))
												{
													for (int i = (randomNums.size() - 1); i >= 0; i--)
													{
														tiles.at(randomNums.at(i)).removeMine();
														randomNums.pop_back();
													}

													for (int i = 0; i < numMines; i++)
													{
														bool numMatches = false;

														// Generates random number
														int randomNum = rand() % numTiles;

														// Sets mines
														if (randomNums.size() == 0)
														{
															tiles.at(randomNum).setMine();
															randomNums.push_back(randomNum);
														}
														// Checks if a random number has already been used
														else
														{
															numMatches = randomNumMatches(randomNums, randomNum);
															while (numMatches == true)
															{
																randomNum = rand() % numTiles;
																numMatches = randomNumMatches(randomNums, randomNum);
															}
															tiles.at(randomNum).setMine();
															randomNums.push_back(randomNum);
														}
													}

													// Resets other info
													for (int i = 0; i < tiles.size(); i++)
													{
														tiles.at(i).setHidden();
														tiles.at(i).removeFlag();
														tiles.at(i).setNeighborMines(0);
														numFlags = 0;
													}

													newGame = true;
													time = chrono::high_resolution_clock::now();
													ePausedTime = chrono::seconds(0);
												}
												// If user clicks leaderboard button
												else if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
												{
													// Variables
													string str;
													leaderboardOpen = true;

													// Sets title text
													titleText.setFont(font);
													titleText.setString("LEADERBOARD");
													titleText.setCharacterSize(20);
													titleText.setFillColor(sf::Color::White);
													titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
													sf::FloatRect textRect = titleText.getLocalBounds();
													titleText.setOrigin((textRect.width / 2.0f), 0);
													titleText.setPosition((lWidth / 2.0f), ((lHeight / 2.0f) - 120));

													// Checks that leaderboard has not been read already
													if (players.size() == 0)
													{
														// Opens file
														inFile.open("files/leaderboard.txt");
														// Checks if file is open
														if (inFile.is_open())
														{
															// Gets ranking text
															while (!inFile.eof())
															{
																inFile >> str;
																strings.push_back(str);
															}

															// Sets times vector
															for (int i = 0; i < strings.size(); i++)
															{
																int index = strings.at(i).find(",");
																string time = strings.at(i).substr(0, index);
																times.push_back(time);
															}

															// Sets players vector
															for (int i = 0; i < strings.size(); i++)
															{
																int index = strings.at(i).find(",");
																string player = strings.at(i).substr(index + 1, strings.at(i).size() - 1);
																players.push_back(player);
															}

															// Sets leaderboard string
															for (int i = 0; i < players.size(); i++)
															{
																leaderboard += to_string(i + 1) + ".\t" + times.at(i) + "\t" + players.at(i) + "\n" + "\n";
															}
														}
														// Closes file
														inFile.close();
													}

													// Renders leaderboard window
													sf::RenderWindow lWindow(sf::VideoMode(lWidth, lHeight), "Minesweeper", sf::Style::Close);
													while (lWindow.isOpen())
													{
														// Focuses leaderboard window
														lWindow.requestFocus();

														// Catches events
														sf::Event lEvent;
														while (lWindow.pollEvent(lEvent))
														{
															// Closes leaderboard
															if (lEvent.type == sf::Event::Closed)
															{
																lWindow.close();
															}
														}
														if (gWindow.pollEvent(event))
														{
															// If user clicks leaderboard button
															if (event.type == sf::Event::MouseButtonPressed)
															{
																if (event.mouseButton.button == sf::Mouse::Left)
																{
																	if (((event.mouseButton.x > (width - 176)) && (event.mouseButton.x < ((width - 176) + 64))) && ((event.mouseButton.y > (32 * (numRows + 0.5f))) && (event.mouseButton.y < ((32 * (numRows + 0.5f)) + 64))))
																	{

																	}
																}
															}
														}

														// Sets leaderboard text
														sf::Text ranking;
														ranking.setFont(font);
														ranking.setString(leaderboard);
														ranking.setCharacterSize(18);
														ranking.setFillColor(sf::Color::White);
														setText(ranking, (lWidth / 2.0f), ((lHeight / 2.0f) + 20));

														// Displays leaderboard
														lWindow.clear(sf::Color::Blue);
														lWindow.draw(titleText);
														lWindow.draw(ranking);
														lWindow.display();
													}
												}
											}
										}
									}
									// If user clicks a hidden tile that is not a mine
									if (((event.mouseButton.x >= tiles.at(i).getPosX()) && (event.mouseButton.x <= (tiles.at(i).getPosX() + 32))) && ((event.mouseButton.y >= tiles.at(i).getPosY()) && (event.mouseButton.y <= (tiles.at(i).getPosY() + 32))) && (tiles.at(i).getFlag() == false))
									{
										if ((tiles.at(i).getHidden() == true) && (tiles.at(i).getMine() == false))
										{
											// Tile is no longer hidden
											tiles.at(i).removeHidden();

											// Get all neighbors
											vector<Tile> neighbors = getNeighbors(tiles.at(i), tiles);

											// Iterates through neighbor vector to determine if neighbor is a mine
											int mineCounter = 0;
											for (int j = 0; j < neighbors.size(); j++)
											{
												if (neighbors.at(j).getMine() == true)
												{
													mineCounter += 1;
												}
											}
											
											// If neighbor is a mine, reveal number
											if (mineCounter > 0)
											{
												tiles.at(i).setNeighborMines(mineCounter);
											}
											// If neighbor is not a mine
											else if (mineCounter == 0)
											{
												for (int j = 0; j < neighbors.size(); j++)
												{
													if ((neighbors.at(j).getHidden() == true) && (neighbors.at(j).getFlag() == false))
													{
														checkNeighbors(&tiles, neighbors.at(j));
													}
												}
											}
										}
									}
								}
							}
						}
					}
					// If user right clicks
					else if (event.mouseButton.button == sf::Mouse::Right)
					{
						// Sets or removes flag
						if ((event.mouseButton.x > 0) && (event.mouseButton.x < (numCols * 32)) && ((event.mouseButton.y > 0) && (event.mouseButton.y < (numRows * 32))))
						{
							for (int i = 0; i < tiles.size(); i++)
							{
								if ((tiles.at(i).getFlag() == false) && (tiles.at(i).getHidden() == true))
								{
									if (((event.mouseButton.x >= tiles.at(i).getPosX()) && (event.mouseButton.x <= (tiles.at(i).getPosX() + 32))) && ((event.mouseButton.y >= tiles.at(i).getPosY()) && (event.mouseButton.y <= (tiles.at(i).getPosY() + 32))))
									{
										tiles.at(i).setFlag();
										numFlags += 1;
									}
								}
								else if (tiles.at(i).getFlag() == true)
								{
									if (((event.mouseButton.x >= tiles.at(i).getPosX()) && (event.mouseButton.x <= (tiles.at(i).getPosX() + 32))) && ((event.mouseButton.y >= tiles.at(i).getPosY()) && (event.mouseButton.y <= (tiles.at(i).getPosY() + 32))))
									{
										tiles.at(i).removeFlag();
										numFlags -= 1;
									}
								}
							}
						}
					}
				}
			}

			// Displays sprites
			gWindow.clear(sf::Color::White);
			for (int i = 0; i < tiles.size(); i++)
			{
				sf::Sprite tileSprite;
				sf::Sprite mineSprite;
				sf::Sprite flagSprite;
				sf::Sprite numTile;
				if ((tiles.at(i).getFlag() == false) && (debug == false) && (tiles.at(i).getHidden() == true))
				{
					tileTexture.loadFromFile("files/images/tile_hidden.png");
					tileSprite.setTexture(tileTexture);
					tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
				}
				else if (tiles.at(i).getFlag() == true)
				{
					tileTexture.loadFromFile("files/images/tile_hidden.png");
					tileSprite.setTexture(tileTexture);
					tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

					flagTexture.loadFromFile("files/images/flag.png");
					flagSprite.setTexture(flagTexture);
					flagSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
				}
				else if ((tiles.at(i).getHidden() == false) && (tiles.at(i).getNeighborMines() > 0))
				{
					tileTexture.loadFromFile("files/images/tile_revealed.png");
					tileSprite.setTexture(tileTexture);
					tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

					string strNum = to_string(tiles.at(i).getNeighborMines());
					string file = "files/images/number_" + strNum + ".png";

					numTileTexture.loadFromFile(file);
					numTile.setTexture(numTileTexture);
					numTile.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
				}
				else if ((tiles.at(i).getHidden() == false) && (tiles.at(i).getNeighborMines() == 0))
				{
					tileTexture.loadFromFile("files/images/tile_revealed.png");
					tileSprite.setTexture(tileTexture);
					tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
				}
				else if (debug == true)
				{
					if (tiles.at(i).getMine() == false)
					{
						tileTexture.loadFromFile("files/images/tile_hidden.png");
						tileSprite.setTexture(tileTexture);
						tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
					}
					else if (tiles.at(i).getMine() == true)
					{
						tileTexture.loadFromFile("files/images/tile_hidden.png");
						tileSprite.setTexture(tileTexture);
						tileSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());

						mineTexture.loadFromFile("files/images/mine.png");
						mineSprite.setTexture(mineTexture);
						mineSprite.setPosition(tiles.at(i).getPosX(), tiles.at(i).getPosY());
					}
				}
				gWindow.draw(tileSprite);
				gWindow.draw(numTile);
				gWindow.draw(mineSprite);
				gWindow.draw(flagSprite);
			}
			gWindow.draw(faceSprite);
			gWindow.draw(debugSprite);
			gWindow.draw(pSprite);
			gWindow.draw(leaderboardSprite);
			// Draws mine counter
			gWindow.draw(mineCounter3);
			gWindow.draw(mineCounter2);
			gWindow.draw(mineCounter1); 
			if (negative == true)
			{
				gWindow.draw(minus);
			}
			// Draws timer
			gWindow.draw(timer4);
			gWindow.draw(timer3);
			gWindow.draw(timer2);
			gWindow.draw(timer1);
			gWindow.display();
		}
	}

	return 0;
}
