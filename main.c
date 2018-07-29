/*Tiffany Xu*/
#include "myLib.h"

enum gameStates {
    STATE_START_SCREEN,
    STATE_INSTRUCTIONS,
    STATE_GAME_START,
    STATE_PLAYING_GAME,
    STATE_LOSE_GAME_RESTART,
    STATE_WIN_GAME_RESTART
};

int main() {
    REG_DISPCNT = MODE3 | BG2_ENABLE;
    enum gameStates state = STATE_START_SCREEN;
    bool a_down = false;
    while(1) {
	waitForVblank();
	switch(state) {
	    case STATE_START_SCREEN:
		drawStart();
		state = STATE_INSTRUCTIONS;
		break;
	    case STATE_INSTRUCTIONS:
		if (KEY_DOWN_NOW(BUTTON_START)) {
		    if (!a_down) {
			a_down = true;
		   	drawInstructions();
		    	state = STATE_GAME_START;
		    } 
		} else {
		    if (a_down) {
			a_down = false;
		    }
		}
		break;
	    case STATE_GAME_START:
		if (KEY_DOWN_NOW(BUTTON_START)) {
 		    if (!a_down) {
			a_down = true;
		        drawGameStart();
			state = STATE_PLAYING_GAME;
		    }
		} else {
		    if (a_down) {
		   	a_down = false;
		    }
		}
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		    state = STATE_START_SCREEN;
		    restartGame();
		}
		break;
	    case STATE_PLAYING_GAME:
		while (1) { 
		    watchActorDude();
		    drawBall();
		    if (checkCollision()) {
			state = STATE_LOSE_GAME_RESTART;			
			break;
		    }
		    if (mainBall.size <= 1) {
			state = STATE_WIN_GAME_RESTART;	
			break;
		    }
		    if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		    
		    break;
		}
		}
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		    state = STATE_START_SCREEN;
		    restartGame();
		}		
		break;

	    case STATE_LOSE_GAME_RESTART:
		drawLoseScreen();
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		    state = STATE_START_SCREEN;
		    restartGame();
		}
		break;
	    case STATE_WIN_GAME_RESTART:
		drawWinScreen();
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		    state = STATE_START_SCREEN;
		    restartGame();
		}
		break;
 
	}
    }
}

