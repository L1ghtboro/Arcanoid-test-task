#pragma once
#include "External/Framework.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <Windows.h>

#define DEFAULT_PLAYER_HEALTH 5
#define PLAYER_DEFAULT_SPEED 40.f

typedef struct Coord {
	int posX;
	int posY;
	Coord() {
		posX = posY = 0;
	}
	Coord(int x, int y) {
		posX = x;
		posY = y;
	}
};

typedef struct Player {
	Sprite* texture;
	int platformSize;
	int heartsLeft;
	float speed;
	Coord playerPos;
	Coord playerMoved;
	bool buffed;
	bool alive;
	std::vector<std::string> boostType;
	Player() {
		texture = nullptr,
		platformSize = 1,
		heartsLeft = DEFAULT_PLAYER_HEALTH,
		speed = PLAYER_DEFAULT_SPEED,
		buffed = false,
		alive = true;
	}
};

typedef struct Ball {
	Sprite* texture;
	float speed;
	bool alive;
	Coord ballPos;
	Coord ballDirection;
	Ball() {
		texture = nullptr;
		speed = 1.f;
		alive = true;
	}
};

typedef struct Brick {
	Sprite* texture;
	Coord brickPos;
	int hitPoints;
	bool alive;
};

class Arcanoid : public Framework {
private:
	int brickSizeX, brickSizeY;
	int indestructibleBricks, destroyedBricks;
	int brickAmount;

	int playerSizeX, playerSizeY;
	
	bool launched;
	bool genOnce;
	bool playerDied;
	bool mouseOverBuy;
	bool gameOver;

	std::vector<std::clock_t> boostTime;
	std::clock_t currentTime;

	int buffStack, combo;
	int playerScore;
	int windowWidth, windowHeight;

	Coord mouseLaunch;
	std::vector<Brick> BrickArray; // here we will save all game bricks
	Player playerPawn;
	Ball playerBall;

	Sprite* backgroundPart; //All sprite variable that we will use in our programm
	Sprite* playerHearts;
	Sprite* playerSprite;
	Sprite* scoreWidget;
	std::vector < Sprite*> scoreNumber;
	Sprite* boostSprite;
	Sprite* instructionSprite;
	//Sprite* ballSprite;
	Sprite* msgWinSprite;
	Sprite* msgWastedSprite;
public:

	Arcanoid();

	Arcanoid(int windowWidth, int windowHeight);

	virtual void PreInit(int& width, int& height, bool& fullscreen);
	
	void PrepareBricks();

	void DrawGameField();

	void DrawBackground();

	void DrawPlayerHearts();

	void DrawPlayer();

	void DrawScoreboard();

	void DrawBall();

	void DrawBuyBoost();

	void DrawInstructions();

	void OnHitBall();

	void Bounce();

	void UpdatePlayer();
	
	bool BoostPlayer();

	void GameEnd();

	virtual bool Init();

	virtual void Close();

	virtual bool Tick();

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative);

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

	virtual void onKeyPressed(FRKey key);

	virtual void onKeyReleased(FRKey key);

	virtual const char* GetTitle() override;
};