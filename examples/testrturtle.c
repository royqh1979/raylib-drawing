//#include "../src/rturtle.h"
//
//int main() {
//	TurtleState ts;
//    initWorld(800,600);
//	
//	setPenSize(20);
//	
//    setSpeed(100);
//	
////	lt(1);
////	for(int i=0;i<4;i++) {
////		fd(150);
////		lt(90);
////	}	
//	fillEllipse(100,100,20,40,RED);
//	
//	drawEllipse(50,50,150,250);
//	
//	fillCircle(-200,-200,40,DARKCYAN);
//	drawCircle(-100,-100,50);
//
//    //setImmediate(TRUE);
//    //setPenColor(BLACK);
//    for(int i=0;i<360;i++) {
//        fd(4);
//        lt(1);
//    }
//	pu();
//	ts=getState();
//	lt(90);
//	fd(10);
//	//fill(SEAGREEN);
//	fillToBorder(getPenColor(),SEAGREEN);
//	setState(ts);
//	pd();
//    waitClick();
//	fd(100);
//	clear();
//    waitClose();
//    return 0;
//}
#include "../src/rturtle.h"
#include <math.h>

int main() {
	initWorld(800,600);
	setAutoUpdateWorld(false);
//	setSpeed(100000000);
//	setImmediate(true);
	setPenSize(1);
	rt(90);
	hide();
	setPenColor(LIGHTRED);
	while (!WindowShouldClose() ){
		
		clear();
		beginFill();
		while(1) {
			fd(200);
			lt(170);
			int x = round(getX());
			int y = round(getY());
			if (x==0 && y==0)
				break;
		}
		endFill(LIGHTYELLOW);
		lt(1);
		waitMilliseconds(1);
		updateWorld();
	}
	waitClick();
	return 0;
}
