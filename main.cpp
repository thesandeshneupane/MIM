#include <iostream>
#include <graphics.h>
#include "drawmenu.h"
#include "pong.h"



using namespace std;
void drawmenu();
void tictactoe();
void pong();
void singleplayer();
void multiplayer();


int main() 
{
	int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    drawmenu();
    getch();
    closegraph();
    




	return 0;
}

