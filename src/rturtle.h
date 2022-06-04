#ifndef RTURTLE_H
#define RTURTLE_H

#include <raylib.h>
#include "rdrawing.h"

/**
 * Press F2 toggle display of grids
 * Press F3 toggle display of the turtle 
 * Press F5 to capture screen
 */

typedef struct
{
	double x;
	double y;
	double angle;
	bool is_pen_down;
	Color pen_color;
	int pen_size;
} TurtleState;

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
	
	void initWorld(int width,int height);
	void closeWorld();
	void fd(double step);
	void forward(double step);
	void bk(double step);
	void backward(double step);
	void lt(double degree);
	void leftTurn(double degree);
	void rt(double degree);
	void rightTurn(double degree);
	void cs();
	void clear();
	void clearScreen();
	void home();
	
	//pause() is defined in linux (unistd.h)
	void waitClick();
	
	void pd();
	void pu();
	void penDown();
	void penUp();
	
	void hide();
	void show();
	void setVisible(bool visible);
	bool isVisible();
	
	void setPenDown(bool isDown);
	bool isPenDown();
	void setPenSize(int size);
	int getPenSize();
	void setPenColor(Color color);
	Color getPenColor();
	
	void setSpeed(int speed);
	int getSpeed();
	void setRewind(bool isRewind);
	bool isRewind();
	void setImmediate(bool isImmediate);
	bool isImmediate();
	
	void setXY(double x, double y);
	void setAngle(double angle);
	void setHeading(double angle);
	void turnTo(double angle);
	void gotoXY(double x, double y);
	double getX();
	double getY();
	double getAngle();
	double getHeading();
	
	TurtleState getState();
	void setState(TurtleState state);
	void faceXY(double x,double y);
	double randBetween(double start,double end);
	void setOrigin(int x, int y);
	void setCaption(const char* title);
	void setBackgroundImage(Image backImg);
	int setBackgroundImageFile(const char* filename);
	void setBackgroundColor(Color color);
	Color getBackgroundColor();
	
	void createGrids(int gridSize, int gridLineWidth, int labelFontSize, Color gridColorX, Color gridColorY);
	void removeGrids();
	
	void fill(Color fillColor);
	void fillToBorder(Color borderColor, Color fillColor);
	
	void drawPoint(int x, int y);
	void drawLine(int x0, int y0, int x1, int y1);
	void drawEllipse(int centerX, int centerY, int radiusX, int radiusY);
	void fillEllipse(int centerX, int centerY, int radiusX, int radiusY, Color fillColor);
	void fillCircle(int centerX, int centerY, int radius, Color fillColor);
	void drawCircle(int centerX, int centerY, int radius, Color fillColor);
	
	
#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif
