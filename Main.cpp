#include "ArcanoidGame.h"

int main(int argc, char* argv[]){
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	return run(new Arcanoid);
}
