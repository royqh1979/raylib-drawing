# raylib-drawing

raylib is a simple and easy-to-use game library, yet it lacks 2d drawing functions. raylib-drawing provides the functions that raylib lacks.

raylib-drawing is composed of two sub libraries:
 - rdrawing.h 2d drawing library
 - rturtle.h turtle graphics library
 
## rdrawing.h

rdrawing.h provides many color definitions and basic 2d drawing functions:

The following program draw and fill a polygon interactively.
```
#include "../src/rdrawing.h"
#include <time.h>

int main() {
	InitWindow(800,600,"test");
	
	Image img=GenImageColor(800,600,WHITE);
	
	SetRandomSeed(time(NULL));
	SetTraceLogLevel(LOG_WARNING);
	int vx[200];
	int vy[200];
	int size=sizeof(vx)/sizeof(int);
	SetTargetFPS(120);
	int num=0;
	while (!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (num<size) {
				vx[num]=GetMouseX();
				vy[num]=GetMouseY();
				if (num>0)
					ImageDrawLine(&img,vx[num-1],vy[num-1],vx[num],vy[num],BLACK);
				ImageDrawPointEx(&img,vx[num],vy[num],3,BLUE);
				num++;
			}
		}
		if (IsKeyPressed(KEY_ENTER)) {
			ImageFillPolygonEx(&img,vx,vy,num,LIGHTRED);
			ImageDrawPolygonEx(&img,vx,vy,num,3,BLACK);
			for (int i=0;i<num;i++) {
				ImageDrawPointEx(&img,vx[i],vy[i],5,BLUE);				
			}
		}
		if (IsKeyPressed(KEY_C)) {
			num = 0;
			ImageClearBackground(&img,WHITE);
		}
		Texture2D texture = LoadTextureFromImage(img);
		BeginDrawing();
		DrawTexture(texture,0,0,WHITE);
		
		EndDrawing();
		UnloadTexture(texture);
	}
	UnloadImage(img);
	return 0;
}
```

| Function | Usage |
|---|---|
| ImageDrawPointEx		| Draw a point to the image |
| ImageDrawArcEx		| Draw an ellipse arc to the image, using the provided line width and color |
| ImageDrawLineEx		| Draw a line to the image, using the provided line width and color |
| ImageDrawPolylineEx	| Draw a polyline to the image, using the provided line width and color |
| ImageDrawRectangleEx  | Draw a rectangle's outline to the image, using the provided line width and color |
| ImageDrawRoundRectEx  | Draw a rounded rectangle's outline to the image, using the provided line width and color |
| ImageDrawCircleEx		| Draw a circle's outline to the image, using the provided line width and color |
| ImageDrawEllipseEx	| Draw an ellipse's outline to the image, using the provided line width and color |
| ImageDrawPolygonEx    | Draw a polygon's outline to the image, using the provided line width and color |
| ImageFillCircleEx		| Fill a circle (no outline), using the provided fill color |
| ImageFillEllipseEx    | Fill an ellipse (no outline), using the provided fill color |
| ImageFillTriangleEx	| Fill a triangle (no outline), using the provided fill color |
| ImageFillRectangleEx	| Fill a rectangle (no outline), using the provided fill color |
| ImageFillRoundRectEx	| Fill a rounded rectangle (no outline), using the provided fill color |
| ImageFillPolygonEx	| Fill a polygon (no outline), using the provided fill color |
| ImageFillSectorEx		| Fill a ellipse sector (no outline), using the provided fill color |
| ImageFloodFill		| Do a flood fill (stop at the pixels has the border color)|
| ImageDrawCubicBezierEx | Draw a cubic bezier curve to the image, using the provided line width and color |
| ImageSetJoinStyle     | Set line join style |
| ImageGetJoinStyle     | Get line join style |

## rturtle.h

rturtle.h is an animated [turtle graphics](https://en.wikipedia.org/wiki/Turtle_graphics) library.

```
#include <rturtle.h>

int main() {
	int n;
	initWorld(800,600);

    setSpeed(500);

	n=50;
	for (int i=0;i<n;i++) {
		for (int j=0;j<4;j++) {
			fd(200);
			lt(90);
		}
		lt(360.0/n);
	}
	waitClick();

	closeWorld();
	return 0;
}
```

It also provided some interactive functions while drawing:
 - Press F2 toggle show/hide of auxilary grids.
 - Press F3 toggle show/hide of the turtle.
 - Press F5 Capture the screen.

| Function | Usage |
|---|---|
| initWorld		| Init the turtle world window |
| closeWorld	| Destory the turtle world window |
| forword / fd	| Move the turtle forward |
| backward / bk	| Move the turtle backward |
| leftTurn / lt | Turn the turtle's heading counter-clockwise (left) |
| rightTurn / rt| Turn the turtle's heading clockwise (right) |
| gotoXY		| let the turtle move to (x,y) |
| turnTo		| turn the turtle's heading to angle |
| faceXY		| turn the turtle heading to (x,y) |
| arc           | Move the turtle to draw an arc |
| clear         | Clear the canvas but don't move the turtle |
| clearScreen / cs | Clear the canvas and set the turtle back to the origin|
| home			| Move the turtle back to the origin, and heading to up |
| waitClick		| Pause the program, until window closed or mouse left-clicked |
| penDown / pd	| set the pen down |
| penUp / pu	| set the pen up (turtle's movement won't have tracks)|
| setPenDown 	| set the pen's donw/up status |
| isPenDown		| query if the pen is down |
| hide / show 	| hide/show the turtle |
| setVisible	| set the turtle's visibility |
| isVisible		| query if the turtle is visible |
| setPenSize	| set the pen's size |
| getPenSize	| get the pen's size |
| setPenColor	| set the pen's color |
| getPenColor	| get the pen's color |
| setSpeed		| set the turtle's move speed |
| getSpeed		| get the turtle's move speed |
| setRewind		| set if the turtle should rewind when hit borders |
| isRewind		| query if the turtle should rewind |
| setImmediate	| set if the turtle's moves should be animated |
| isImmediate	| query if the turtle's moves should be animated |
| setXY			| set the turtle's position to (x,y) |
| setHeading	| set the turtle's heading |
| getX			| get the turtle's position's x-coordination |
| getY			| get the turtle's position's y-coordination |
| getAngle / getHeading | get the turtle's heading angle|
| getState		| get the turtle's state |
| setState		| set the turtle's state |
| setOrigin		| set the origin to (x,y) |
| setCaption	| set the window's caption |
| setBackgroundImage | set the window's background image |
| setBackgroundImageFile | Load image from the file, and set the window's background image to it |
| setBackgroundColor | set the canvas's background color |
| getBackgroundColor | get the canvas's background color |
| createGrids		| create auxilary grids |
| removeGrids		| remove auxilary grids | 
| beginFill / endFill |  fill the polygon generated by turtle's move |
| fill				| flood fill (stop at pixels that has different color from the current pixel) |
| fillToBorder		| flood fill (stop at pixels that has the border color) |
| drawPoint			| draw a point |
| drawLine			| draw a line  |
| drawEllipse		| draw an ellipse's outline |
| fillEllipse		| fill an ellipse |
