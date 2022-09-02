#include "ArcanoidGame.h"

inline constexpr std::uint32_t istr(const char* str, std::uint32_t hash = 2166136261UL) { // converting text to int (for switch)
	return *str ? istr(str + 1, (hash ^ *str) * 16777619ULL) : hash;
}

Arcanoid::Arcanoid() { 
	this->windowWidth = 800;
	this->windowHeight = 600;
	//For better flexibility we will calculate all our sizes based on  window X,Y size
	this->brickAmount = 14;
	this->brickSizeX = this->windowWidth / 30;
	this->brickSizeY = this->windowHeight * 2 / 100;

	this->playerSizeX = this->windowWidth / 14;
	this->playerSizeY = this->windowHeight / 42;

	this->playerBall.ballPos.posX = this->windowWidth / 2 - this->windowWidth / 84; //Ball starter position is slightly upper than player
	this->playerBall.ballPos.posY = (this->windowHeight / 7) * 6 - this->windowHeight / 100;
	
	this->combo = 0;
	this->buffStack = 0;

	this->playerScore = 0;
	this->launched = false; //If ball have been launched (game start)

	this->genOnce = true; //To make our bricks computed only once (not per tick)
	this->playerDied = false;
	this->gameOver = false;
	
	this->indestructibleBricks = 0;
	this->destroyedBricks = 0;
}

Arcanoid::Arcanoid(int windowWidth, int windowHeight) { //Another constructor if we will send data out from class
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	this->brickAmount = 14;
	this->brickSizeX = this->windowWidth / 30;
	this->brickSizeY = this->windowHeight * 2 / 100;

	this->playerSizeX = this->windowWidth / 14;
	this->playerSizeY = this->windowHeight / 42;

	this->playerBall.ballPos.posX = this->windowWidth / 2 - this->windowWidth / 84;
	this->playerBall.ballPos.posY = (this->windowHeight / 7) * 6 - this->windowHeight / 100;

	this->combo = 0;
	this->buffStack = 0;

	this->playerScore = 0;
	this->launched = false;

	this->genOnce = true;
	this->playerDied = false;
	this->gameOver = false;

	this->indestructibleBricks = 0;
	this->destroyedBricks = 0;
}

void Arcanoid::PreInit(int& width, int& height, bool& fullscreen) {
	width = this->windowWidth;
	height = this->windowHeight;
	fullscreen = false;
}

void Arcanoid::PrepareBricks() {
	Sprite* brickTypes[] = { createSprite("External/data/01-Breakout-Tiles.png")
						, createSprite("External/data/11-Breakout-Tiles.png")
						, createSprite("External/data/03-Breakout-Tiles.png")
						, createSprite("External/data/13-Breakout-Tiles.png") }; //Making array of brick types ( brick0 and brick1 will have same amount of hit points but another texture)
	for (int arrayCounter = 0; arrayCounter < 4; arrayCounter++)
		setSpriteSize(brickTypes[arrayCounter], this->brickSizeX, this->brickSizeY); 
	srand(time(NULL));
	for (int bricksY = 0; bricksY < 3; bricksY++) {
		int choosenType = rand() % 3; //Making random choice of bricks
		for (int bricksX = 0; bricksX < this->brickAmount; bricksX++) {
			Brick brickify; //Temp variable
			brickify.texture = nullptr, brickify.hitPoints = 0, brickify.alive = false;

			int ifIndestructible = rand() % 100; 

			if (!(ifIndestructible > 5)) //Making 5% chance of occuring indestructible block
				brickify.texture = brickTypes[3], brickify.hitPoints = 99999, brickify.alive = true, this->indestructibleBricks++;
			if (brickify.texture == nullptr) //Fill other bricks (i could make it else instead of another if, but in case if it bugged with auto filling, so we can secure out game)
				brickify.texture = brickTypes[choosenType], brickify.hitPoints = (choosenType) ? choosenType : 1, brickify.alive = true;

			brickify.brickPos.posX = this->windowWidth / 2 + this->brickSizeX * bricksX - this->brickSizeX * this->brickAmount / 2;
			brickify.brickPos.posY = this->windowHeight / 4 + bricksY * this->windowHeight / 18;
			BrickArray.push_back(brickify);
		}
	}
}

void Arcanoid::DrawGameField() {
	if (genOnce)
		this->PrepareBricks(), genOnce = false; //Preparing our brick array 
	for (int bricksY = 0; bricksY < 3; bricksY++) {
		for (int bricksX = 0; bricksX < this->brickAmount; bricksX++) {
			int brickCounter = bricksX + brickAmount * bricksY; //I didn't want to make another variable but if we use full sentance in array it bugs
			if(BrickArray[brickCounter].alive)
				drawSprite(BrickArray[brickCounter].texture, BrickArray[brickCounter].brickPos.posX, BrickArray[brickCounter].brickPos.posY);
		}
	}
}

void Arcanoid::DrawBackground() {
	int backgroundScale = this->windowHeight / 9;
	setSpriteSize(backgroundPart, this->windowHeight / 9, this->windowHeight / 9);
	for (int borderX = 0; borderX * backgroundScale < 1920; borderX++) //Filling game field with background (just white picture(didn't find a tool to make it just #fff))
		for (int borderY = 0; borderY * backgroundScale < 1080; borderY++)
			drawSprite(backgroundPart, borderX * backgroundScale, borderY * backgroundScale);
}

void Arcanoid::DrawPlayerHearts() {
	setSpriteSize(playerHearts, this->windowWidth / 50, this->windowWidth / 50);
	for (int hearts = 0; hearts < playerPawn.heartsLeft; hearts++) // in this case we tracking player hearts so if it amount will decrease it will decrease in UI too
		drawSprite(playerHearts, this->windowWidth / 2 + this->brickSizeX * this->brickAmount / 2 + this->windowWidth / 50 * hearts + this->playerSizeX, this->windowHeight*1/5);
}

void Arcanoid::DrawPlayer() {
	setSpriteSize(playerSprite, this->playerSizeX, this->playerSizeY);
	playerPawn.texture = playerSprite;
	playerPawn.playerPos.posX = this->windowWidth / 2 - this->playerSizeX / 2, playerPawn.playerPos.posY = this->windowHeight*7/8;
	drawSprite(playerPawn.texture, playerPawn.playerPos.posX + playerPawn.playerMoved.posX, playerPawn.playerPos.posY + playerPawn.playerMoved.posY);
}

void Arcanoid::DrawScoreboard() {
	if (this->playerScore < 0)
		this->playerScore = 0;

	setSpriteSize(scoreWidget, this->windowWidth / 5, this->windowHeight / 6);

	drawSprite(scoreWidget, 0, 0);

	if (!playerScore) {
		setSpriteSize(scoreNumber[playerScore], windowWidth / 16, windowHeight / 16);
		drawSprite(scoreNumber[playerScore], windowWidth / 100, windowHeight / 18);
		return;
	}

	int scoreTemp = this->playerScore;

	for (int scoreAmount = int(log10(playerScore) + 1) - 1; scoreAmount > -1 ; scoreAmount--) { //Calculating player score and drawing it in UI
		setSpriteSize(scoreNumber[scoreTemp%10], this->windowWidth / 16, this->windowHeight / 16);
		drawSprite(scoreNumber[scoreTemp % 10], this->windowWidth / 100 + this->windowWidth / 16 * scoreAmount, this->windowHeight / 18); //here bug
		scoreTemp /= 10;
	}
}

void Arcanoid::DrawBuyBoost() {
	setSpriteSize(boostSprite, this->windowWidth / 6, this->windowHeight / 7);
	drawSprite(boostSprite, this->windowWidth - this->windowWidth / 6, this->windowHeight - this->windowHeight / 7);
}

void Arcanoid::DrawInstructions() {
	setSpriteSize(instructionSprite, this->windowWidth / 6, this->windowHeight / 6);
	drawSprite(instructionSprite, 0, windowHeight*6/8);
}

void Arcanoid::DrawBall() {
	Sprite* ballSprite = createSprite("External/data/58-Breakout-Tiles.png");
	setSpriteSize(ballSprite, this->windowWidth / 42, this->windowHeight / 42);
	if(!this->launched) // if ball not launched then it is attached to player
		drawSprite(ballSprite, this->windowWidth / 2 - this->windowWidth / 84 + playerPawn.playerMoved.posX, (this->windowHeight / 7) * 6 - this->windowHeight / 100);
	else if (this->launched) {
		this->Bounce();
		this->playerBall.ballPos.posX += this->playerBall.speed * this->playerBall.ballDirection.posX;
		this->playerBall.ballPos.posY += this->playerBall.speed * this->playerBall.ballDirection.posY;
		drawSprite(ballSprite, this->playerBall.ballPos.posX, this->playerBall.ballPos.posY);
	}
}

void Arcanoid::Bounce() {
	if (this->playerBall.ballPos.posX > this->windowWidth / 2 + this->brickSizeX * this->brickAmount / 2 - this->windowWidth / 42 or this->playerBall.ballPos.posX < this->windowWidth / 2 - this->brickSizeX * this->brickAmount / 2) {
		this->playerBall.ballDirection.posX *= -1; // Bounce to opposite direction when hit game border
	}
	else if (this->playerBall.ballPos.posY < this->windowHeight / 150) {
		this->playerBall.ballDirection.posY *= -1; // Bounce back if hit game top border
	}
	else if (this->playerBall.ballPos.posX + this->windowWidth / 42 > this->playerPawn.playerPos.posX + this->playerPawn.playerMoved.posX and this->playerBall.ballPos.posX < this->playerPawn.playerPos.posX + this->playerPawn.playerMoved.posX + this->playerSizeX and (this->playerBall.ballPos.posY + this->windowHeight / 42 > this->playerPawn.playerPos.posY - this->windowHeight/200 and this->playerBall.ballPos.posY + this->windowHeight / 42 < this->playerPawn.playerPos.posY + this->windowHeight / 200)) {
		this->playerBall.ballDirection.posY *= -1; //bounce from platform
		combo = 0; //start combo from again
	}
}

void Arcanoid::OnHitBall() {
	for (int brickCounter = 0; brickCounter < BrickArray.size(); brickCounter++) { //going through bricks (bad decision)
		if (((this->playerBall.ballPos.posX > this->BrickArray[brickCounter].brickPos.posX and this->playerBall.ballPos.posX < this->BrickArray[brickCounter].brickPos.posX + this->brickSizeX and (this->playerBall.ballPos.posY > this->BrickArray[brickCounter].brickPos.posY and this->playerBall.ballPos.posY < this->BrickArray[brickCounter].brickPos.posY + this->brickSizeY)) or (this->playerBall.ballPos.posX + this->windowWidth / 42 > this->BrickArray[brickCounter].brickPos.posX and (this->playerBall.ballPos.posX + this->windowWidth / 42 < this->BrickArray[brickCounter].brickPos.posX + this->brickSizeX and (this->playerBall.ballPos.posY + this->windowHeight / 42 > this->BrickArray[brickCounter].brickPos.posY and this->playerBall.ballPos.posY + this->windowHeight / 42 < this->BrickArray[brickCounter].brickPos.posY + this->brickSizeY)))) and BrickArray[brickCounter].alive) {
			this->playerBall.ballDirection.posY *= -1; // we check if we hit brick and than bounce back 
			BrickArray[brickCounter].hitPoints--;
			if (!BrickArray[brickCounter].hitPoints) {
				combo++;
				BrickArray[brickCounter].alive = false; //making brick dead
				(combo > 2) ? playerScore += 20 * 3 : playerScore += 20; // if we doing a combo we doing our point x3
				destroyedBricks++;
			}
		}
	}
}

void Arcanoid::UpdatePlayer() {
	if (this->playerBall.ballPos.posY > this->windowHeight) { 
		this->launched = false; // Update that we can shoot ball again after we died
		if (!this->playerDied) {
			this->playerScore -= 100; // take away 100 points when player died
			this->playerPawn.heartsLeft--;
			this->playerDied = true;
			this->combo = 0;
		}
	}
}

bool Arcanoid::BoostPlayer() { 
	currentTime = clock(); // making clock each tick 
	if (this->playerPawn.buffed) {
		if ((currentTime / CLOCKS_PER_SEC - boostTime.front() / CLOCKS_PER_SEC) == 20) { //checking if difference is 20 sec
			boostTime.erase(boostTime.begin()); // removing boost timer
			switch (istr(playerPawn.boostType.front().c_str())) { // searching which type of boost we used
				case istr("faster"):
					this->playerPawn.speed -= PLAYER_DEFAULT_SPEED / 4;
					break;
				case istr("slower"):
					this->playerPawn.speed += this->playerPawn.speed * 2 / 5;
					break;
			}
			if(boostTime.empty()) // if there no boosts
				playerPawn.buffed = false;
			playerPawn.boostType.erase(playerPawn.boostType.begin());
			this->buffStack--;
			return true;
		}
	}
	return false;
}

bool Arcanoid::Init() {
	backgroundPart = createSprite("External/Assets/background_custom.png");

	playerHearts = createSprite("External/data/60-Breakout-Tiles.png");

	playerSprite = createSprite("External/data/50-Breakout-Tiles.png");

	scoreWidget = createSprite("External/Assets/brickEmpty.png");
	
	scoreNumber = { createSprite("External/Assets/Numbers/numb0.png")
							, createSprite("External/Assets/Numbers/numb1.png")
							, createSprite("External/Assets/Numbers/numb2.png")
							, createSprite("External/Assets/Numbers/numb3.png")
							, createSprite("External/Assets/Numbers/numb4.png")
							, createSprite("External/Assets/Numbers/numb5.png")
							, createSprite("External/Assets/Numbers/numb6.png")
							, createSprite("External/Assets/Numbers/numb7.png")
							, createSprite("External/Assets/Numbers/numb8.png")
							, createSprite("External/Assets/Numbers/numb9.png") }; //Custom numbers array

	boostSprite = createSprite("External/Assets/brickBuy.png");

	instructionSprite = createSprite("External/Assets/instructions.png");

	//ballSprite = createSprite("External/data/58-Breakout-Tiles.png");

	msgWinSprite = createSprite("External/Assets/brickWon.png");

	msgWastedSprite = createSprite("External/Assets/brickWasted.png");

	return true;
}

void Arcanoid::Close() {

}

void Arcanoid::GameEnd() {
	if (this->destroyedBricks + this->indestructibleBricks == this->brickAmount * 3) {
		setSpriteSize(msgWinSprite, this->windowWidth /4, this->windowHeight / 4);
		drawSprite(msgWinSprite, this->windowWidth / 2 - this->windowWidth / 8, this->windowWidth / 2 - this->windowWidth / 8);
		this->playerBall.speed = 0.f;
		this->gameOver = true;
	}
	else if (!this->playerPawn.heartsLeft) {
		setSpriteSize(msgWastedSprite, this->windowWidth / 4, this->windowHeight / 4);
		drawSprite(msgWastedSprite, this->windowWidth / 2 - this->windowWidth / 8, this->windowWidth / 2 - this->windowWidth / 8);
		this->playerBall.speed = 0.f;
		this->gameOver = true;
	}
}

bool Arcanoid::Tick() {
	//drawTestBackground();
	
	DrawBackground();
	DrawGameField();

	DrawPlayer();
	DrawPlayerHearts();
	DrawScoreboard();
	DrawInstructions();
	DrawBuyBoost();
	DrawBall();

	UpdatePlayer();
	OnHitBall();
	BoostPlayer();
	GameEnd();

	return false;
}

void Arcanoid::onMouseMove(int x, int y, int xrelative, int yrelative) {
	if ((x < this->windowWidth - this->windowWidth / 6) and (y < this->windowHeight - this->windowHeight / 7)) //tracking if mouse over section to buy boost
		this->mouseOverBuy = false;
	else
		this->mouseOverBuy = true;
	
	this->mouseLaunch.posX = x; //saving (x, y) of mouse position
	this->mouseLaunch.posY = y;
}

void Arcanoid::onMouseButtonClick(FRMouseButton button, bool isReleased) {
	if ((int)button == 2 and this->mouseOverBuy and isReleased and this->buffStack != 4 and this->playerScore >= 20) { //buy boost
		this->playerScore -= 20;
		this->playerPawn.buffed = true;
		this->boostTime.push_back(clock()); //adding to vector time when last boost obtained
		this->buffStack++;
		switch (rand()%2) { 
			case 0:			
				this->playerPawn.speed += PLAYER_DEFAULT_SPEED/4;
				this->playerPawn.boostType.push_back("faster"); //faster
				break;
			case 1:
				if (this->playerPawn.speed - PLAYER_DEFAULT_SPEED * 2 / 5 >= PLAYER_DEFAULT_SPEED / 2) //slower by 40% but not bigger than 50% of default speed
					this->playerPawn.speed -= PLAYER_DEFAULT_SPEED * 2 / 5; 
				this->playerPawn.boostType.push_back("slower");
				break;
		}
	}
	else if (!int(button) and !this->gameOver) { //launching ball by mouse
		srand(time(NULL));
		if (!this->launched) {
			this->playerBall.speed = 1.f;
			this->playerBall.ballPos.posX = this->windowWidth / 2 - this->windowWidth / 84 + playerPawn.playerMoved.posX; //ball will be launched from the place where player landed it
			this->playerBall.ballPos.posY = (this->windowHeight / 7) * 6 - this->windowHeight / 100;
			this->playerBall.ballDirection.posX = (this->mouseLaunch.posX > this->windowWidth/2) ? (mouseLaunch.posX / this->playerBall.ballPos.posX) : -(this->playerBall.ballPos.posY / mouseLaunch.posY);
			this->playerBall.ballDirection.posY = -(this->playerBall.ballPos.posY / mouseLaunch.posY); //far cursor from player the more speed he will obtain
		}
		this->launched = true;
		this->playerDied = false;
	}
}

void Arcanoid::onKeyPressed(FRKey key) {
	switch (int(key)) {
		case 0:
			if (playerPawn.playerMoved.posX < this->brickSizeX * this->brickAmount / 2 - this->playerSizeX / 2) //here checker is not a good idea because we calculate with playerMoved
				playerPawn.playerMoved.posX += playerPawn.speed;
			break;
		case 1:
			if (playerPawn.playerMoved.posX > -(this->brickSizeX * this->brickAmount / 2 - this->playerSizeX / 2))
				playerPawn.playerMoved.posX -= playerPawn.speed;
			break;
		case 2:
			if (this->gameOver) { //restarting game if it ends
				BrickArray.erase(BrickArray.begin(), BrickArray.end());
				genOnce = true;
				gameOver = false;
				destroyedBricks = 0;
				indestructibleBricks = 0;
				playerScore = 0;
				this->playerPawn.heartsLeft = DEFAULT_PLAYER_HEALTH;
				this->buffStack = 0;
				this->combo = 0;
				this->playerBall.ballPos.posX = this->windowWidth / 2 - this->windowWidth / 84 + playerPawn.playerMoved.posX;
				this->launched = false;
			}
			break;
		case 3:
			break;
	}
}

void Arcanoid::onKeyReleased(FRKey key) {

}

const char* Arcanoid::GetTitle(){
	return "Arcanoid";
}