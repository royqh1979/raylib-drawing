#include "../src/rturtle.h"

int main() {
	TurtleState ts;
    initWorld(800,600);
	
    setSpeed(500);
	fillEllipse(100,100,20,40,RED);
	
	drawEllipse(50,50,150,250);
	
	fillCircle(-200,-200,40,DARKCYAN);
	drawCircle(-100,-100,50,LIGHTRED);

    //setImmediate(TRUE);
    //setPenColor(BLACK);
    for(int i=0;i<360;i++) {
        fd(4);
        lt(1);
    }
	pu();
	ts=getState();
	lt(90);
	fd(10);
	//fill(SEAGREEN);
	fillToBorder(getPenColor(),SEAGREEN);
	setState(ts);
	pd();
    pause();
	clearScreen();
    hide();
    pause();
    return 0;
}

