#include <iostream>
#include <graphics.h>
#include "drawmenu.h"
#include "pong.h"

//void drawmenu();

//bool isClickedInRect(int x1, int y1, int x2, int y2, int x, int y) {
//    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
//}

using namespace std;
void pong();


void pong() 
{
    setbkcolor(0); // Black background
    cleardevice();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(200, 100, "Multiplayer in Multiverse");
    outtextxy(130, 150, "Choose the mode you want to Play");
    rectangle(150, 200, 370, 250);
    outtextxy(170, 220, "Single Player");
    rectangle(150, 300, 370, 350);
    outtextxy(170, 320, "Multi Player");
    rectangle(180, 385, 350, 430);
    outtextxy(200, 400, "Main Menu");
    rectangle(555, 395, 480, 420);
    outtextxy(490, 400, "Exit");
    

}

