#include <iostream>
#include <graphics.h>
#include "drawmenu.h"
#include "pong.h"


using namespace std;
void drawmenu();

bool isClickedInRect(int x1, int y1, int x2, int y2, int x, int y) {
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}


void drawmenu() 
{
    setbkcolor(0); // Black background
    cleardevice();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(200, 100, "Multiplayer in Multiverse");
    outtextxy(130, 150, "Choose the game you want to Play");
    rectangle(150, 200, 350, 250);
    outtextxy(170, 220, "Tic-Tac-Toe");
    rectangle(150, 300, 350, 350);
    outtextxy(210, 320, "Pong");
    rectangle(370, 375, 450, 400);
    outtextxy(380, 380, "Exit");
    int x, y;
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (isClickedInRect(150, 200, 350, 250, x, y)) return drawmenu(); // tic-tac-toe
            if (isClickedInRect(150, 300, 350, 350, x, y)) return pong(); // pong
            if (isClickedInRect(370, 375, 450, 400, x, y)) // exit
			{
                closegraph();
                exit(0);
            }
        }
    }
}
