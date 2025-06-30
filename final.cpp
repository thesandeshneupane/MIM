#include <graphics.h>
#include <winbgim.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

const int paddleWidth = 20;
const int paddleHeight = 150;
const int paddleSpeed = 10;
const int ballRadius = 20;
const int winningScore = 3;

int screenWidth, screenHeight;
int ballX, ballY, ballSpeedX, ballSpeedY;
int leftPaddleY, rightPaddleY;
int leftScore, rightScore;
char player1[20] = "Player 1", player2[20] = "Player 2";
clock_t lastSpeedIncreaseTime;
int gameMode = 2; // 1 = Single Player, 2 = Multiplayer

void getNameInput(char* nameBuffer, char* label, int x, int y) {
    settextstyle(3, 0, 2);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(WHITE);

    char input[20] = "";
    int len = 0;

    while (true) {
        setfillstyle(SOLID_FILL, BLACK);
        bar(x, y, x + 500, y + 100);
        outtextxy(x, y, label);
        rectangle(x, y + 40, x + 400, y + 80);
        outtextxy(x + 10, y + 50, input);

        if (kbhit()) {
            char ch = getch();
            if (ch == 13) break; // Enter
            else if (ch == 8 && len > 0) { // Backspace
                input[--len] = '\0';
            } else if (len < 19 && ch >= 32 && ch <= 126) {
                input[len++] = ch;
                input[len] = '\0';
            }
        }
        delay(50);
    }

    strcpy(nameBuffer, input);
}

int showGameModeMenu() {
    int choice = 0;
    int menuWidth = GetSystemMetrics(SM_CXSCREEN) ;
    int menuHeight = GetSystemMetrics(SM_CYSCREEN)*0.9 ;

    initwindow(menuWidth, menuHeight, "Select Game Mode");
    setbkcolor(BLACK);
    cleardevice();

    // Title
    settextstyle(3, 0, 5);
    setcolor(WHITE);
    char titleText[] = "Select Game Mode";
    outtextxy((menuWidth - textwidth(titleText)) / 2, 100, titleText);

    // Button dimensions
    int buttonWidth = 300, buttonHeight = 80;
	int singleX = (menuWidth - buttonWidth) / 2;
	int singleY = 220;
	int multiY = 330;
	int mainMenuY = 440;
	int exitY  = 550;


    // Text style for buttons
    settextstyle(3, 0, 5);  // smaller so text fits

    // Draw all three buttons
    char singleText[] = "Single Player";
    char multiText[] = "Multiplayer";
    char exitText[]  = "Exit";

    int buttons[4][2] = { {singleX, singleY}, {singleX, multiY}, {singleX, mainMenuY}, {singleX, exitY} };
	char* labels[4] = { singleText, multiText, "Main Menu", exitText };


    for (int i = 0; i < 4; ++i) {
        setfillstyle(SOLID_FILL, BLACK);
        bar(buttons[i][0], buttons[i][1],
            buttons[i][0] + buttonWidth, buttons[i][1] + buttonHeight);
        setcolor(WHITE);
        rectangle(buttons[i][0], buttons[i][1],
                  buttons[i][0] + buttonWidth, buttons[i][1] + buttonHeight);
        outtextxy(
            buttons[i][0] + (buttonWidth - textwidth(labels[i])) / 2,
            buttons[i][1] + (buttonHeight - textheight(labels[i])) / 2,
            labels[i]
        );
    }

    // Mouse interaction
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx, my;
            getmouseclick(WM_LBUTTONDOWN, mx, my);

            // Single Player
            if (mx >= singleX && mx <= singleX + buttonWidth &&
                my >= singleY && my <= singleY + buttonHeight) {
                choice = 1;
                break;
            }

            // Multiplayer
            if (mx >= singleX && mx <= singleX + buttonWidth &&
                my >= multiY && my <= multiY + buttonHeight) {
                choice = 2;
                break;
            }
            // Main Menu
			if (mx >= singleX && mx <= singleX + buttonWidth &&
    			my >= mainMenuY && my <= mainMenuY + buttonHeight) {
    			choice = 3; // Return to front menu
   				break;
			}


            // Exit button
            if (mx >= singleX && mx <= singleX + buttonWidth &&
                my >= exitY && my <= exitY + buttonHeight) {
                // Confirmation
                int boxX = menuWidth / 2 - 200;
                int boxY = menuHeight / 2 - 100;
                int boxW = 400;
                int boxH = 200;

                setfillstyle(SOLID_FILL, DARKGRAY);
                bar(boxX, boxY, boxX + boxW, boxY + boxH);
                setcolor(WHITE);
                rectangle(boxX, boxY, boxX + boxW, boxH + boxY);

                settextstyle(3, 0, 2);
                char confirmText[] = "Are you sure you want to exit?";
                outtextxy(boxX + (boxW - textwidth(confirmText)) / 2, boxY + 40, confirmText);

                // Yes / No buttons
                int btnW = 120, btnH = 50;
                int yesX = boxX + 40, yesY = boxY + 120;
                int noX = boxX + boxW - 40 - btnW, noY = yesY;

                setfillstyle(SOLID_FILL, BLACK);
                bar(yesX, yesY, yesX + btnW, yesY + btnH);
                rectangle(yesX, yesY, yesX + btnW, yesY + btnH);
                outtextxy(yesX + (btnW - textwidth("Yes")) / 2, yesY + 15, "Yes");

                bar(noX, noY, noX + btnW, noY + btnH);
                rectangle(noX, noY, noX + btnW, noY + btnH);
                outtextxy(noX + (btnW - textwidth("No")) / 2, noY + 15, "No");

                // Wait for Yes or No click
                while (true) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        int cx, cy;
                        getmouseclick(WM_LBUTTONDOWN, cx, cy);

                        if (cx >= yesX && cx <= yesX + btnW && cy >= yesY && cy <= yesY + btnH) {
                            closegraph();
                            exit(0);
                        } else if (cx >= noX && cx <= noX + btnW && cy >= noY && cy <= noY + btnH) {
                            // Redraw menu
                            cleardevice();
                            setbkcolor(BLACK);
                            settextstyle(3, 0, 5);
                            outtextxy((menuWidth - textwidth(titleText)) / 2, 100, titleText);

                            settextstyle(3, 0, 5);
                            for (int i = 0; i < 3; ++i) {
                                setfillstyle(SOLID_FILL, BLACK);
                                bar(buttons[i][0], buttons[i][1],
                                    buttons[i][0] + buttonWidth, buttons[i][1] + buttonHeight);
                                rectangle(buttons[i][0], buttons[i][1],
                                          buttons[i][0] + buttonWidth, buttons[i][1] + buttonHeight);
                                outtextxy(
                                    buttons[i][0] + (buttonWidth - textwidth(labels[i])) / 2,
                                    buttons[i][1] + (buttonHeight - textheight(labels[i])) / 2,
                                    labels[i]
                                );
                            }
                            break;
                        }
                    }
                    delay(20);
                }
            }
        }
        delay(20);
    }

    delay(200);
    closegraph();
    return choice;
}




void initializeGame() {
    leftScore = 0;
    rightScore = 0;
    leftPaddleY = (screenHeight - paddleHeight) / 2;
    rightPaddleY = (screenHeight - paddleHeight) / 2;
    ballX = screenWidth / 2;
    ballY = screenHeight / 2;
    ballSpeedX = (rand() % 2 == 0) ? 5 : -5;
    ballSpeedY = (rand() % 2 == 0) ? 3 : -3;
    lastSpeedIncreaseTime = clock();
}

void drawBackground() {
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(0, 0, screenWidth / 2, screenHeight);
    setfillstyle(SOLID_FILL, GREEN);
    bar(screenWidth / 2, 0, screenWidth, screenHeight);
}

void drawCenterLine() {
    setcolor(WHITE);
    for (int y = 0; y < screenHeight; y += 20)
        line(screenWidth / 2, y, screenWidth / 2, y + 10);
}

void drawPaddles() {
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    rectangle(30, leftPaddleY, 30 + paddleWidth, leftPaddleY + paddleHeight);
    floodfill(31, leftPaddleY + 1, RED);

    int rightX = screenWidth - 30 - paddleWidth;
    rectangle(rightX, rightPaddleY, rightX + paddleWidth, rightPaddleY + paddleHeight);
    floodfill(rightX + 1, rightPaddleY + 1, RED);
}

void drawBall() {
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    circle(ballX, ballY, ballRadius);
    floodfill(ballX, ballY, RED);
}

void drawScores() {
    setcolor(WHITE);
    char scoreText[100];
    sprintf(scoreText, "%s: %d    %s: %d", player1, leftScore, player2, rightScore);
    outtextxy((screenWidth - textwidth(scoreText)) / 2, 20, scoreText);
}

void resetBall(bool scoredByLeft) {
    ballX = screenWidth / 2;
    ballY = screenHeight / 2;
    ballSpeedX = scoredByLeft ? 5 : -5;
    ballSpeedY = (rand() % 2 == 0) ? 3 : -3;
    lastSpeedIncreaseTime = clock();
}

bool showGameOver(const char* winnerName) {
    Beep(1200, 400);
    setvisualpage(0);
    setactivepage(0);
    cleardevice();
    setcolor(WHITE);
    settextstyle(3, 0, 2);
    char msg[100];
    sprintf(msg, "Game Over! %s Wins!", winnerName);
    outtextxy((screenWidth - textwidth(msg)) / 2, screenHeight / 2 - 30, msg);
    outtextxy((screenWidth - textwidth("Press R to Restart or ESC to Exit")) / 2, screenHeight / 2 + 10,
              "Press R to Restart or ESC to Exit");

    while (1) {
        if (GetAsyncKeyState('R') & 0x8000) return true;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return false;
        delay(50);
    }
}

void showCountdown() {
    settextstyle(3, 0, 5);
    setcolor(WHITE);
    for (int i = 3; i > 0; i--) {
        cleardevice();
        drawBackground();
        drawCenterLine();
        char num[2];
        sprintf(num, "%d", i);
        outtextxy((screenWidth - textwidth(num)) / 2, screenHeight / 2 - 50, num);
        delay(700);
    }
    cleardevice();
    drawBackground();
    drawCenterLine();
    outtextxy((screenWidth - textwidth("Start!")) / 2, screenHeight / 2 - 50, "Start!");
    delay(700);
    cleardevice();
}

int showFrontMenu() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.95;
    initwindow(screenWidth, screenHeight, "Multiplayer in Multiverse");
    setbkcolor(BLACK);
    cleardevice();

    settextstyle(3, 0, 8);
    setcolor(WHITE);
    char title[] = "MULTIPLAYER IN MULTIVERSE";
    outtextxy((screenWidth - textwidth(title)) / 2, 100, title);

    // Button settings
    int buttonWidth = 400, buttonHeight = 100;
    int startX = (screenWidth - buttonWidth) / 2;
    int pongY = 250, tttY = 380, exitY = 510;

    char* options[] = {"Pong", "Tic Tac Toe", "Exit"};
    int positions[] = {pongY, tttY, exitY};

    settextstyle(3, 0, 5);
    for (int i = 0; i < 3; i++) {
        setfillstyle(SOLID_FILL, BLACK);
        bar(startX, positions[i], startX + buttonWidth, positions[i] + buttonHeight);
        setcolor(WHITE);
        rectangle(startX, positions[i], startX + buttonWidth, positions[i] + buttonHeight);
        outtextxy(
            startX + (buttonWidth - textwidth(options[i])) / 2,
            positions[i] + (buttonHeight - textheight(options[i])) / 2,
            options[i]
        );
    }

    // Mouse interaction
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx, my;
            getmouseclick(WM_LBUTTONDOWN, mx, my);

            if (mx >= startX && mx <= startX + buttonWidth) {
                if (my >= pongY && my <= pongY + buttonHeight) {
                    closegraph();
                    return 1; // Pong
                } else if (my >= tttY && my <= tttY + buttonHeight) {
                    closegraph();
                    return 2; // Tic Tac Toe
                } else if (my >= exitY && my <= exitY + buttonHeight) {
                    closegraph();
                    exit(0);
                }
            }
        }
        delay(20);
    }
}

int main() {
	
	int frontChoice = showFrontMenu();
	
	
    screenWidth = GetSystemMetrics(SM_CXSCREEN) ;
    screenHeight = GetSystemMetrics(SM_CYSCREEN) *0.95;

    while (true) {
    gameMode = showGameModeMenu();
    if (gameMode == 3) {
        frontChoice = showFrontMenu();
        continue;
    } else {
        break;
    }
}

    initwindow(screenWidth, screenHeight, "Pong Game");
    setbkcolor(BLACK);
    cleardevice();

    getNameInput(player1, "Enter Player 1 Name:", screenWidth/2-250, screenHeight/3);
    if (gameMode == 1) {
        strcpy(player2, "Bot");
    } else {
        getNameInput(player2, "Enter Player 2 Name:", screenWidth/2-250, screenHeight/2);
    }

    bool running = true;
    while (running) {
        initializeGame();
        showCountdown();

        int currentPage = 0;
        while (1) {
            setactivepage(currentPage);
            setvisualpage(1 - currentPage);
            cleardevice();

            drawBackground();
            drawCenterLine();
            drawPaddles();
            drawBall();
            drawScores();

            ballX += ballSpeedX;
            ballY += ballSpeedY;

            if (ballY - ballRadius <= 0 || ballY + ballRadius >= screenHeight)
                ballSpeedY = -ballSpeedY;

            if (ballX - ballRadius <= 30 + paddleWidth &&
                ballY >= leftPaddleY && ballY <= leftPaddleY + paddleHeight) {
                ballSpeedX = -ballSpeedX;
                ballX = 30 + paddleWidth + ballRadius;
                Beep(800, 50);
            }

            int rightX = screenWidth - 30 - paddleWidth;
            if (ballX + ballRadius >= rightX &&
                ballY >= rightPaddleY && ballY <= rightPaddleY + paddleHeight) {
                ballSpeedX = -ballSpeedX;
                ballX = rightX - ballRadius;
                Beep(800, 50);
            }

            if (((clock() - lastSpeedIncreaseTime) / CLOCKS_PER_SEC) >= 5) {
                ballSpeedX += (ballSpeedX > 0) ? 1 : -1;
                ballSpeedY += (ballSpeedY > 0) ? 1 : -1;
                lastSpeedIncreaseTime = clock();
            }

            if (ballX < 0) {
                rightScore++;
                Beep(1000, 200);
                if (rightScore >= winningScore) {
                    if (!showGameOver(player2)) running = false;
                    break;
                }
                resetBall(false);
                showCountdown();
            } else if (ballX > screenWidth) {
                leftScore++;
                Beep(1000, 200);
                if (leftScore >= winningScore) {
                    if (!showGameOver(player1)) running = false;
                    break;
                }
                resetBall(true);
                showCountdown();
            }

            // Player 1
            if (GetAsyncKeyState('W') & 0x8000) {
                leftPaddleY -= paddleSpeed;
                if (leftPaddleY < 0) leftPaddleY = 0;
            }
            if (GetAsyncKeyState('S') & 0x8000) {
                leftPaddleY += paddleSpeed;
                if (leftPaddleY + paddleHeight > screenHeight)
                    leftPaddleY = screenHeight - paddleHeight;
            }

            // Player 2 or Bot
            if (gameMode == 2) {
                if (GetAsyncKeyState(VK_UP) & 0x8000) {
                    rightPaddleY -= paddleSpeed;
                    if (rightPaddleY < 0) rightPaddleY = 0;
                }
                if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    rightPaddleY += paddleSpeed;
                    if (rightPaddleY + paddleHeight > screenHeight)
                        rightPaddleY = screenHeight - paddleHeight;
                }
            } else {
                int centerBot = rightPaddleY + paddleHeight / 2;
                int botSpeed = paddleSpeed - 2;
                if (ballY < centerBot)
                    rightPaddleY = max(0, rightPaddleY - botSpeed);
                else if (ballY > centerBot)
                    rightPaddleY = min(screenHeight - paddleHeight, rightPaddleY + botSpeed);
            }

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                running = false;
                break;
            }

            delay(20);
            currentPage = 1 - currentPage;
        }
    }

    closegraph();
    return 0;
}

