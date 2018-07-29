/*Tiffany Xu*/
#include "myLib.h"

int actorDudeX = 102; //top row of buzz
int actorDudeY = 30;  //first column of buzz
int score = 0;
static int actorDudeV = 0; // vertical velocity for jumping

/* erase Buzz so you can draw new Buzz at new position */
void clearActorDude(int x, int y) {
    drawRect(x, y, 30, 23, BLACK);
}

/* "watches" Buzz to see if he should move based on specific key presses */
void watchActorDude() {
    //this prints the score
    char buffer[51];
    sprintf(buffer, "Score: %d", score);
    drawRect(140, 3, 12, 54, BLACK);
    drawString(142, 5, buffer, YELLOW);

    clearActorDude(actorDudeX, actorDudeY);
    
    //move left
    if (KEY_DOWN_NOW(BUTTON_LEFT)) {
	if (actorDudeY - 1 >= 0) {
            actorDudeY -= 1;
	}
    }
    //move right
    if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
	if (actorDudeY + 31 <= 240) {
            actorDudeY += 1;
	}
    }
    //jump
    if (actorDudeX == 102) {
	actorDudeV = 0;
	if (KEY_DOWN_NOW(BUTTON_A)) actorDudeV = -5;
    }

    if (actorDudeX == 80) actorDudeV = 0;

    actorDudeX += actorDudeV;

    if (actorDudeX < 80) actorDudeX = 80;
    if (actorDudeX > 102) actorDudeX = 102;
 
    drawImage3(actorDudeX + 1, actorDudeY, 30, 30, actorDude);
    //mimics gravity; going up slowing down, going down speeding up
    actorDudeV += 1;
}

/* checks if Buzz and the ball collide; returns 1 if lose, 0 otherwise */
int checkCollision() {
    //Buzz earns a point
    if ((mainBall.row + mainBall.size) >= actorDudeX && actorDudeX < 102) {
	if ((mainBall.col >= actorDudeY && mainBall.col <= (actorDudeY + 30)) || (mainBall.col + mainBall.size >= actorDudeY && mainBall.col + mainBall.size <= (actorDudeY + 30)))  {
	    //erase ball before it rebounds
	    drawRect(mainBall.row, mainBall.col, mainBall.size, mainBall.size, BLACK);	    
	    mainBall.cd = -mainBall.cd;
	    mainBall.rd = -mainBall.rd;
	    mainBall.size -= 5;
	    score += 1;
	    return 0;
	}
    //Buzz loses
    } else if ((mainBall.row + mainBall.size) >= actorDudeX && actorDudeX == 102) {
	if ((mainBall.col >= actorDudeY && mainBall.col <= (actorDudeY + 30)) || (mainBall.col + mainBall.size >= actorDudeY && mainBall.col + mainBall.size <= (actorDudeY + 30))) {
	    return 1;
	}
    } return 0;
}


