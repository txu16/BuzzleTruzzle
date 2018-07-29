/*Tiffany Xu*/
#include "myLib.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
volatile unsigned short black = BLACK;
volatile unsigned short red = RED;
fakeBall mainBall = {.row = 80, .col = 120, .rd = 1, .cd = 1, .size = 20};

/* drawimage3

* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3 (int r, int c, int width, int height, const u16* image) {
    for (int row  = 0; row < height; row++) {    
	DMA[MODE3].src = image + (row * width);
	DMA[MODE3].dst = videoBuffer + (240 * (r + row) + c);
	DMA[MODE3].cnt = width | DMA_ON; 
    }
}

void waitForVblank() {
    while(*SCANLINECOUNTER > 160);
    while(*SCANLINECOUNTER < 160);
}

void setPixel(int row, int col, u16 color) {
    videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
    for(int r = 0; r < height; r++) {
	DMA[MODE3].src = &color;
	DMA[MODE3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
	DMA[MODE3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }
}

void drawChar(int row, int col, char ch, unsigned short color) {
    for(int r = 0; r<8; r++) {
	for(int c=0; c<6; c++) {
	    if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
		setPixel(row+r, col+c, color);
	    }
	}
    }
}

void drawString(int row, int col, char *str, unsigned short color) {
    while(*str) {
	drawChar(row, col, *str++, color);
	col += 6;
    }	
}
/* draws the starting screen of the game */
void drawStart() {
    drawImage3(0, 0, STARTSCREEN_WIDTH, STARTSCREEN_HEIGHT, startScreen);
    drawString(125, 22, "Press Enter", BLACK);
    drawString(140, 22, "to begin!", BLACK);
}

/* draws the instruction screen which happens after starting screen */
void drawInstructions() {
    drawImage3(0, 0, BRICK_WIDTH, BRICK_HEIGHT, brick);
    drawString(30, 105, "RULES", RED);
    drawString(45, 35, "Use left and right arrow keys", BLACK);
    drawString(60, 35, "to move Buzz around. Press Z", BLACK);
    drawString(75, 35, "to jump and minimize the ball.", BLACK);
    drawString(90, 35, "If the ball hits you when you", BLACK);
    drawString(105, 35, "are not jumping, you DIE!.", BLACK);
    drawString(120, 62, "Press ENTER to start.", YELLOW);

}

/* draws the actual active game beginning with game background and buzz */
void drawGameStart() {
    drawImage3(0, 0, GAMEBACKGROUND_WIDTH, GAMEBACKGROUND_HEIGHT, gameBackground);
    drawImage3(103, 30, ACTORDUDE_WIDTH, ACTORDUDE_HEIGHT, actorDude);
}

/* drawing the ball that bounces around the screen for you to destroy */
void drawBall() {
    drawRect(mainBall.row, mainBall.col, mainBall.size, mainBall.size, black);
    mainBall.row = mainBall.row + mainBall.rd;
    mainBall.col += mainBall.cd;
    if(mainBall.row < 3) {
	mainBall.row = 3;
	mainBall.rd = -mainBall.rd;
    }
    if(mainBall.row > 133-mainBall.size) {
	mainBall.row = 133-mainBall.size;
	mainBall.rd = -mainBall.rd;
    }
    if(mainBall.col < 0) {
	mainBall.col = 0;
	mainBall.cd = -mainBall.cd;
    }
    if(mainBall.col > 239-mainBall.size+1) {
	mainBall.col = 239-mainBall.size+1;
	mainBall.cd = -mainBall.cd;
    }
    drawRect(mainBall.row, mainBall.col, mainBall.size, mainBall.size, red);
    waitForVblank();
}

/* when Select button is clicked to go to starting screen, this is called 
to reset all variables so when the game is started again, it starts fresh */
void restartGame() {
    actorDudeX = 102; //row
    actorDudeY = 30;  //column
    score = 0;
    drawGameStart();
    mainBall.row = 80;
    mainBall.col = 120;
    mainBall.rd = 1;
    mainBall.cd = 1;
    mainBall.size = 20;
}

/* when the ball hits Buzz, this losing image is drawn */
void drawLoseScreen() {
    drawImage3(0, 0, LOSESCREEN_WIDTH, LOSESCREEN_HEIGHT, loseScreen);
    drawString(30, 10, "Click BACKSPACE to restart.", black); //not drawing
}

/* when Buzz successfully destroys the ball, this winning image is drawn */
void drawWinScreen() {
    drawImage3(0, 0, YOUWIN_WIDTH, YOUWIN_HEIGHT, youWin);
    drawString(100, 10, "CONGRATS U ROCK.", WHITE);
    drawString(130, 10, "Click BACKSPACE.", WHITE);
}


