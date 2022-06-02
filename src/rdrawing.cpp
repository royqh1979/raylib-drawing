#include "rdrawing.h"
#include <math.h>
#include <algorithm>
#include <queue>
#include <string.h>

#define COLOR_EQUAL(col1, col2) ((col1.r == col2.r) && (col1.g == col2.g) && (col1.b == col2.b) && (col1.a == col2.a))


void ImageDrawPointEx(Image* dst,int x, int y, int pointSize, Color color) {
	ImageFillCircleEx(dst,x,y,pointSize/2,color);
}

static void doDrawLineLow(Image* image, int x0, int y0, int x1, int y1, int lineWidth, Color color) {
	if (x1==x0)
		return;
	if (x1<x0) {
		std::swap(x0,x1);
		std::swap(y0,y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi;
	if (dy<0) {
		yi=-1;
		dy=-dy;
	} else {
		yi=1;
	}
	int D = (2*dy)-dx;
	int y=y0;
	int	w = lineWidth/2.0*dx/sqrt(dx*dx+dy*dy); 
	for (int x=x0;x<=x1;x++) {
		for (int yy=y-w;yy<=y+w;yy++) {
			ImageDrawPixel(image,
				x,yy,color);
		}
		if (D>0) {
			y+=yi;
			D+=2*(dy-dx);
		}else{
			D+=2*dy;
		}
	}
}

static void doDrawLineHigh(Image* image,int x0, int y0, int x1, int y1, int lineWidth, Color color) {
	if (y1==y0)
		return;
	if (y1<y0) {
		std::swap(x0,x1);
		std::swap(y0,y1);
	}		
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi;
	if (dx<0) {
		xi = -1;
		dx = -dx;
	} else {
		xi = 1;
	}
	int D = (2*dx) - dy;
	int x = x0;
	int	w = lineWidth/2.0*dy/sqrt(dx*dx+dy*dy);
	for (int y=y0;y<=y1;y++) {
		for (int xx=x-w;xx<=x+w;xx++) {
			ImageDrawPixel(image,
				xx,y,color);
		}
		if (D>0) {
			x+=xi;
			D+=2*(dx-dy);
		}else{
			D+=2*dx;
		}
	}	
} 

void ImageDrawLineEx(Image* dst,int x0, int y0, int x1, int y1, int lineWidth, Color color) {
	if (abs(y1-y0)<abs(x1-x0)) {
		doDrawLineLow(dst,x0,y0,x1,y1,lineWidth,color);
	} else {
		doDrawLineHigh(dst,x0,y0,x1,y1,lineWidth, color);
	}	
}

static void draw4EllipseBordersH(Image* dst,int CX, int CY, int X1, int X2, int Y, Color color) {
	if (X2<X1)
		std::swap(X1,X2);
	int x1 = CX-X2;
	int x2 = CX-X1;
	int x3 = CX+X1;
	int x4 = CX+X2;
	x1 = std::min(std::max(0,x1),dst->width-1);
	x2 = std::min(std::max(0,x2),dst->width-1);
	x3 = std::min(std::max(0,x3),dst->width-1);
	x4 = std::min(std::max(0,x4),dst->width-1);
	int y=CY-Y;
	if (y>=0 && y<dst->height) {
		for (int x=x1;x<=x2;x++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
		for (int x=x3;x<=x4;x++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
	}
	y=CY+Y;	
	if (y>=0 && y<dst->height) {
		for (int x=x1;x<=x2;x++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
		for (int x=x3;x<=x4;x++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
	}
}

static void draw4EllipseBordersV(Image* dst, int CX, int CY, int X, int Y1, int Y2, Color color) {
	if (Y2<Y1)
		std::swap(Y1,Y2);
	int y1 = CY-Y2;
	int y2 = CY-Y1;
	int y3 = CY+Y1;
	int y4 = CY+Y2;
	y1 = std::min(std::max(0,y1),dst->width-1);
	y2 = std::min(std::max(0,y2),dst->width-1);
	y3 = std::min(std::max(0,y3),dst->width-1);
	y4 = std::min(std::max(0,y4),dst->width-1);
	int x=CX-X;
	if (x>=0 && x<dst->width) {
		for (int y=y1;y<=y2;y++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
		for (int y=y3;y<=y4;y++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
	}
	x=CX+X;
	if (x>=0 && x<dst->width) {
		for (int y=y1;y<=y2;y++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
		for (int y=y3;y<=y4;y++) {
			ImageDrawPixel(dst, x, y ,color);		
		}
	}
}

//ellipse algorithm from  https://web.archive.org/web/20120225095359/http://homepage.smc.edu/kennedy_john/belipse.pdf

void ImageDrawEllipseEx(Image* dst, int cx, int cy, int radiusX, int radiusY, int lineWidth, Color color){
	int radiusX1=radiusX-lineWidth/2;
	int radiusX2=radiusX+lineWidth/2;
	int radiusY1=radiusY-lineWidth/2;
	int radiusY2=radiusY+lineWidth/2;
	int twoASquare1=2*radiusX1*radiusX1;
	int twoBSquare1=2*radiusY1*radiusY1;
	int twoASquare2=2*radiusX2*radiusX2;
	int twoBSquare2=2*radiusY2*radiusY2;
	int x1=radiusX1;
	int x2=radiusX2;
	int y=0;	
	int XChange1 = radiusY1*radiusY1*(1-2*radiusX1);
	int YChange1 = radiusX1*radiusX1;
	int ellipseError1 = 0;
	int stoppingX1 = twoBSquare1*radiusX1;
	int stoppingY1 = 0;
	int XChange2 = radiusY2*radiusY2*(1-2*radiusX2);
	int YChange2 = radiusX2*radiusX2;
	int ellipseError2 = 0;
	int stoppingX2 = twoBSquare2*radiusX2;
	int stoppingY2 = 0;
	
	//first stage, y'>-1
	while (stoppingX2>=stoppingY2) {
		draw4EllipseBordersH(dst, cx,cy,x1,x2,y,color);
		y+=1;
		if (stoppingX1>=stoppingY1) {
			stoppingY1 += twoASquare1;
			ellipseError1 += YChange1;
			YChange1 += twoASquare1;
			if ((2*ellipseError1+XChange1)>0){
				x1-=1;
				stoppingX1 -= twoBSquare1;
				ellipseError1 += XChange1;
				XChange1+=twoBSquare1;
			}
		}
		stoppingY2 += twoASquare2;
		ellipseError2 += YChange2;
		YChange2 += twoASquare2;
		if ((2*ellipseError2+XChange2)>0){
			x2-=1;
			stoppingX2 -= twoBSquare2;
			ellipseError2 += XChange2;
			XChange2+=twoBSquare2;
		}
	}
	//second stage, y'<-1
	int x=0;
	int y1=radiusY1;
	int y2=radiusY2;
	XChange1 = radiusY1 * radiusY1;
	YChange1 = radiusX1 * radiusX1 * (1-2*radiusY1);
	ellipseError1 = 0;
	stoppingX1 = 0;
	stoppingY1 = twoASquare1*radiusY1;
	XChange2 = radiusY2 * radiusY2;
	YChange2 = radiusX2 * radiusX2 * (1-2*radiusY2);
	ellipseError2 = 0;
	stoppingX2 = 0;
	stoppingY2 = twoASquare2*radiusY2;
	while (stoppingX2<=stoppingY2) {
		draw4EllipseBordersV(dst, cx,cy,x,y1,y2,color);
		x++;
		if (stoppingX1<=stoppingY1) {
			stoppingX1+=twoBSquare1;
			ellipseError1 += XChange1;
			XChange1+=twoBSquare1;
			if ((2*ellipseError1+YChange1)>0) {
				y1-=1;
				stoppingY1-=twoASquare1;
				ellipseError1+=YChange1;
				YChange1+=twoASquare1;
			}							
		}
		stoppingX2+=twoBSquare2;
		ellipseError2 += XChange2;
		XChange2+=twoBSquare2;
		if ((2*ellipseError2+YChange2)>0) {
			y2-=1;
			stoppingY2-=twoASquare2;
			ellipseError2+=YChange2;
			YChange2+=twoASquare2;
		}				
	}	
}

void ImageDrawCircleEx(Image* dst, int cx, int cy, int radius, int lineWidth, Color color) {
	int radius1=radius-lineWidth/2;
	int radius2=radius+lineWidth/2;
	int twoSquare1=2*radius1*radius1;
	int twoSquare2=2*radius2*radius2;
	int x1=radius1;
	int x2=radius2;
	int y=0;	
	int XChange1 = radius1*radius1*(1-2*radius1);
	int YChange1 = radius1*radius1;
	int ellipseError1 = 0;
	int stoppingX1 = twoSquare1*radius1;
	int stoppingY1 = 0;
	int XChange2 = radius2*radius2*(1-2*radius2);
	int YChange2 = radius2*radius2;
	int ellipseError2 = 0;
	int stoppingX2 = twoSquare2*radius2;
	int stoppingY2 = 0;
	
	while (stoppingX2>=stoppingY2) {
		draw4EllipseBordersH(dst, cx,cy,x1,x2,y,color);
		draw4EllipseBordersV(dst, cx,cy,y,x1,x2,color);
		y+=1;
		if (stoppingX1>=stoppingY1) {
			stoppingY1 += twoSquare1;
			ellipseError1 += YChange1;
			YChange1 += twoSquare1;
			if ((2*ellipseError1+XChange1)>0){
				x1-=1;
				stoppingX1 -= twoSquare1;
				ellipseError1 += XChange1;
				XChange1+=twoSquare1;
			}
		}
		stoppingY2 += twoSquare2;
		ellipseError2 += YChange2;
		YChange2 += twoSquare2;
		if ((2*ellipseError2+XChange2)>0){
			x2-=1;
			stoppingX2 -= twoSquare2;
			ellipseError2 += XChange2;
			XChange2+=twoSquare2;
		}
	}
}

static void fill2EllipseLines(Image* dst, int CX, int CY, int X, int Y, Color color) {
	int x1 = CX-X;
	int x2 = CX+X;
	if (x2<x1)
		std::swap(x1,x2);
	x1 = std::max(0,x1);
	x2 = std::min(x2,dst->width-1);
	int y1=CY-Y;
	int y2=CY+Y;
	if (y1>=0 && y1<dst->height) {
		for (int x=x1;x<=x2;x++) {
			ImageDrawPixel(dst, x, y1 ,color);		
		}
	} 
	if (y2>=0 && y2<dst->height) {
		for (int x=x1;x<=x2;x++) {
			ImageDrawPixel(dst, x, y2 ,color);		
		}
	} 
}

void ImageFillEllipseEx(Image* dst, int cx, int cy, int radiusX, int radiusY, Color fillColor) {
	int twoASquare=2*radiusX*radiusX;
	int twoBSquare=2*radiusY*radiusY;
	int x=radiusX;
	int y=0;
	int XChange = radiusY*radiusY*(1-2*radiusX);
	int YChange = radiusX*radiusX;
	int ellipseError = 0;
	int stoppingX = twoBSquare*radiusX;
	int stoppingY = 0;
	//first stage, y'>-1
	while (stoppingX>=stoppingY) {
		fill2EllipseLines(dst, cx,cy,x,y,fillColor);
		y+=1;
		stoppingY += twoASquare;
		ellipseError += YChange;
		YChange += twoASquare;
		if ((2*ellipseError+XChange)>0){
			x-=1;
			stoppingX -= twoBSquare;
			ellipseError += XChange;
			XChange+=twoBSquare;
		}
	}
	//second stage, y'<-1
	x=0;
	y=radiusY;
	XChange = radiusY * radiusY;
	YChange = radiusX * radiusX * (1-2*radiusY);
	ellipseError = 0;
	stoppingX = 0;
	stoppingY = twoASquare*radiusY;
	while (stoppingX<=stoppingY) {
		x++;
		stoppingX+=twoBSquare;
		ellipseError += XChange;
		XChange+=twoBSquare;
		if ((2*ellipseError+YChange)>0) {
			fill2EllipseLines(dst, cx, cy, x, y, fillColor);
			y-=1;
			stoppingY-=twoASquare;
			ellipseError+=YChange;
			YChange+=twoASquare;
		}				
	}	
}

void ImageFillCircleEx(Image* dst, int cx, int cy, int radius, Color fillColor){
	int twoASquare=2*radius*radius;
	int x=radius;
	int y=0;
	int XChange = radius*radius*(1-2*radius);
	int YChange = radius*radius;
	int ellipseError = 0;
	int stoppingX = twoASquare*radius;
	int stoppingY = 0;
	//first stage, y'>-1
	while (stoppingX>=stoppingY) {
		fill2EllipseLines(dst,cx,cy,x,y,fillColor);
		y+=1;
		stoppingY += twoASquare;
		ellipseError += YChange;
		YChange += twoASquare;
		if ((2*ellipseError+XChange)>0){
			fill2EllipseLines(dst,cx,cy,y,x,fillColor);
			x-=1;
			stoppingX -= twoASquare;
			ellipseError += XChange;
			XChange+=twoASquare;
		}
	}	
}

void ImageFloodFill(Image* dst,int x, int y, Color borderColor, Color fillColor) {
	int imgWidth = dst->width;
	int imgHeight = dst->height;
	std::queue<int> queueX;
	std::queue<int> queueY;
	queueX.push(x);
	queueY.push(y);
	int flagsSize=sizeof(int)*dst->width*dst->height;
	int * flags=(int*)malloc(flagsSize);
	memset(flags,0,flagsSize);
	while (!queueX.empty()) {
		x = queueX.front();
		y = queueY.front();
		queueX.pop();
		queueY.pop();
		if (x<0 || x>=imgWidth)
			continue;
		if (y<0 || y>=imgHeight)
			continue;
		if (flags[y*imgWidth+x]==1)
			continue;
		Color cc = GetImageColor(*dst,x,y);
		if (COLOR_EQUAL(borderColor, cc))
			continue;
		ImageDrawPixel(dst,x,y,fillColor);
		flags[y*imgWidth+x]=1;
		queueX.push(x);queueY.push(y+1);
		queueX.push(x+1);queueY.push(y);
		queueX.push(x);queueY.push(y-1);
		queueX.push(x-1);queueY.push(y);
	}
	free(flags);

}
