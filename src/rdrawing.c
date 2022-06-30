#include "rdrawing.h"
#include "utils.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_EQUAL(col1, col2) ((col1.r == col2.r) && (col1.g == col2.g) && (col1.b == col2.b) && (col1.a == col2.a))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

static ImageJoinStyle _default_image_join_style = IMAGE_JOIN_ROUND;

static void swapInt(int* p1, int*p2) {
	int t = *p2;
	*p2 = *p1;
	*p1 = t;
}

void ImageDrawPointEx(Image* dst,int x, int y, int pointSize, Color color) {
	if (pointSize<=1) 
		ImageDrawPixel(dst,x,y,color);
	else
		ImageFillCircleEx(dst,x,y,pointSize/2,color);
}

//static void doDrawLineLow(Image* image, int x0, int y0, int x1, int y1, int lineWidth, Color color) {
//	if (x1==x0)
//		return;
//	if (x1<x0) {
//		swapInt(&x0,&x1);
//		swapInt(&y0,&y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int yi;
//	if (dy<0) {
//		yi=-1;
//		dy=-dy;
//	} else {
//		yi=1;
//	}
//	int D = (2*dy)-dx;
//	int D_delta1=2*(dy-dx);
//	int D_delta2=2*dy;
//	int y=y0;
//	float lwv = lineWidth*sqrt(dx*dx+dy*dy)/dx;
//	int	w = lwv/2.0;
//	float b = lineWidth*dy / dx;
//	float end_dx=b*lineWidth/2/lwv;
//	float end_dy=b*b/2/lwv;
//	int end_mid_x0 = x0+end_dy;
//	int end_mid_x1 = x1-end_dy;
//	
//	for (int x=x0;x<=x1;x++) {
//		if (x>=end_mid_x0 && x<=end_mid_x1) {
//			for (int yy=y-w;yy<=y+w;yy++) {
//				ImageDrawPixel(image,
//					x,yy,color);
//			}
//		}
//		if (D>0) {
//			y+=yi;
//			D+=D_delta1;
//		}else{
//			D+=D_delta2;
//		}
//	}
//	if (yi>0) {
//		int vx[3];
//		int vy[3];
//		vx[0]=end_mid_x0;
//		vy[0]=y0+end_dy-w;
//		vx[1]=end_mid_x0;
//		vy[1]=y0+end_dy+w;
//		vx[2]=x0-end_dx;
//		vy[2]=y0-end_dy+w;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//		vx[0]=end_mid_x1;
//		vy[0]=y1-end_dy-w;
//		vx[1]=end_mid_x1;
//		vy[1]=y1-end_dy+w;
//		vx[2]=x1+end_dx;
//		vy[2]=y1+end_dy-w;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//	} else {
//		int vx[3];
//		int vy[3];
//		vx[0]=end_mid_x0;
//		vy[0]=y0-end_dy-w;
//		vx[1]=end_mid_x0;
//		vy[1]=y0-end_dy+w;
//		vx[2]=x0-end_dx;
//		vy[2]=y0+end_dy-w;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//		vx[0]=end_mid_x1;
//		vy[0]=y1+end_dy-w;
//		vx[1]=end_mid_x1;
//		vy[1]=y1+end_dy+w;
//		vx[2]=x1+end_dx;
//		vy[2]=y1-end_dy+w;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//	}
//	
//}
//
//static void doDrawLineHigh(Image* image,int x0, int y0, int x1, int y1, int lineWidth, Color color) {
//	if (y1==y0)
//		return;
//	if (y1<y0) {
//		swapInt(&x0,&x1);
//		swapInt(&y0,&y1);
//	}		
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int xi;
//	if (dx<0) {
//		xi = -1;
//		dx = -dx;
//	} else {
//		xi = 1;
//	}
//	int D = (2*dx) - dy;
//	int D_delta1=2*(dx-dy);
//	int D_delta2=2*dx;
//	
//	int x = x0;
//	float lwh = lineWidth*sqrt(dx*dx+dy*dy)/dy;
//	int	w = lwh/2.0;
//	float b = lineWidth*dx / dy;
//	float end_dy=b*lineWidth/2/lwh;
//	float end_dx=b*b/2/lwh;
//	int end_mid_y0 = y0+end_dy;
//	int end_mid_y1 = y1-end_dy;
//	
//	//	TraceLog(LOG_WARNING, "%f, %f, %f,%d,%d,%d,%d\n", lwh,b,end_dy, y0,y1,end_mid_y0,end_mid_y1);
//	for (int y=y0;y<=y1;y++) {
//		if (y>=end_mid_y0 && y<=end_mid_y1) {
//			for (int xx=x-w;xx<=x+w;xx++) {
//				ImageDrawPixel(image,
//					xx,y,color);
//			}
//		}
//		if (D>0) {
//			x+=xi;
//			D+=D_delta1;
//		}else{
//			D+=D_delta2;
//		}
//	}
//	if (xi>0) {
//		int vx[3];
//		int vy[3];
//		vx[0]=x0+end_dx-w;
//		vy[0]=end_mid_y0;
//		vx[1]=x0+end_dx+w;
//		vy[1]=end_mid_y0;
//		vx[2]=x0-end_dx+w;
//		vy[2]=y0-end_dy;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//		vx[0]=x1-end_dx-w;
//		vy[0]=end_mid_y1;
//		vx[1]=x1-end_dx+w;
//		vy[1]=end_mid_y1;
//		vx[2]=x1+end_dx-w;
//		vy[2]=y1+end_dy;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//	} else {
//		int vx[3];
//		int vy[3];
//		vx[0]=x0-end_dx-w;
//		vy[0]=end_mid_y0;
//		vx[1]=x0-end_dx+w;
//		vy[1]=end_mid_y0;
//		vx[2]=x0+end_dx-w;
//		vy[2]=y0-end_dy;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//		vx[0]=x1+end_dx-w;
//		vy[0]=end_mid_y1;
//		vx[1]=x1+end_dx+w;
//		vy[1]=end_mid_y1;
//		vx[2]=x1-end_dx+w;
//		vy[2]=y1+end_dy;
//		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
//	}
//} 

void ImageDrawLineEx(Image* dst,int x0, int y0, int x1, int y1, int lineWidth, Color color) {
	if (lineWidth==1) {
		ImageDrawLine(dst,x0,y0,x1,y1,color);
		return;
	}
	//	if (abs(y1-y0)<abs(x1-x0)) {
	//		doDrawLineLow(dst,x0,y0,x1,y1,lineWidth,color);
	//	} else {
	//		doDrawLineHigh(dst,x0,y0,x1,y1,lineWidth, color);
	//	}	
	if (x0==x1 && y0==y1)
		return;
	if (lineWidth<1)
		return;
	int vx[4];
	int vy[4];
	if (x0>x1) {
		swapInt(&x0,&x1);
		swapInt(&y0,&y1);
	}
	int dx = x1-x0;
	int dy = y0-y1;
	float wx = lineWidth * dy / sqrt(dx*dx+dy*dy) / 2;
	float wy = lineWidth * dx / sqrt(dx*dx+dy*dy) / 2;
	vx[0]=x0-wx;
	vy[0]=y0-wy;
	vx[1]=x0+wx;
	vy[1]=y0+wy;
	vx[2]=x1+wx;
	vy[2]=y1+wy;
	vx[3]=x1-wx;
	vy[3]=y1-wy;	
	ImageFillPolygonEx(dst,vx,vy,4,color);
	if (_default_image_join_style==IMAGE_JOIN_ROUND) {
		ImageDrawPointEx(dst,x0,y0,lineWidth,color);
		ImageDrawPointEx(dst,x1,y1,lineWidth,color);
	}
}

static void draw4EllipseBordersH(Image* dst,int CX, int CY, int X1, int X2, int Y, Color color) {
	if (X2<X1)
		swapInt(&X1,&X2);
	int x1 = CX-X2;
	int x2 = CX-X1;
	int x3 = CX+X1;
	int x4 = CX+X2;
	x1 = MIN(MAX(0,x1),dst->width-1);
	x2 = MIN(MAX(0,x2),dst->width-1);
	x3 = MIN(MAX(0,x3),dst->width-1);
	x4 = MIN(MAX(0,x4),dst->width-1);
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
		swapInt(&Y1,&Y2);
	int y1 = CY-Y2;
	int y2 = CY-Y1;
	int y3 = CY+Y1;
	int y4 = CY+Y2;
	y1 = MIN(MAX(0,y1),dst->width-1);
	y2 = MIN(MAX(0,y2),dst->width-1);
	y3 = MIN(MAX(0,y3),dst->width-1);
	y4 = MIN(MAX(0,y4),dst->width-1);
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
		swapInt(&x1,&x2);
	x1 = MAX(0,x1);
	x2 = MIN(x2,dst->width-1);
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
	if (radius<1)
		return;
	int twoSquare=2*radius*radius;
	int x=radius;
	int y=0;
	int XChange = radius*radius*(1-2*radius);
	int YChange = radius*radius;
	int ellipseError = 0;
	int stoppingX = twoSquare*radius;
	int stoppingY = 0;
	//first stage, y'>-1
	while (stoppingX>=stoppingY) {
		fill2EllipseLines(dst,cx,cy,x,y,fillColor);
		y+=1;
		stoppingY += twoSquare;
		ellipseError += YChange;
		YChange += twoSquare;
		if ((2*ellipseError+XChange)>0){
			fill2EllipseLines(dst,cx,cy,y,x,fillColor);
			x-=1;
			stoppingX -= twoSquare;
			ellipseError += XChange;
			XChange+=twoSquare;
		}
	}	
}

void ImageFloodFill(Image* dst,int x, int y, Color borderColor, Color fillColor) {
	int imgWidth = dst->width;
	int imgHeight = dst->height;
	
	IntQueue queueX;
	IntQueue queueY;
	IntQueue_init(&queueX);
	IntQueue_init(&queueY);
	IntQueue_push(&queueX,x);
	IntQueue_push(&queueY,y);
	int flagsSize=sizeof(int)*dst->width*dst->height;
	int * flags=(int*)malloc(flagsSize);
	memset(flags,0,flagsSize);
	while (!IntQueue_empty(&queueX)) {
		x = IntQueue_pop(&queueX);
		y = IntQueue_pop(&queueY);
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
		IntQueue_push(&queueX,x);IntQueue_push(&queueY,y-1);
		IntQueue_push(&queueX,x);IntQueue_push(&queueY,y+1);
		IntQueue_push(&queueX,x+1);IntQueue_push(&queueY,y);
		IntQueue_push(&queueX,x-1);IntQueue_push(&queueY,y);
	}
	free(flags);
	IntQueue_free(&queueX);
	IntQueue_free(&queueY);
}

static void doDrawFillLineH(Image* dst,int x1,int x2, int y,Color color) {
	if (x1>x2)
		swapInt(&x1,&x2);
	for (int x=x1;x<=x2;x++) {
		ImageDrawPixel(dst,x,y,color);
	}
}

static void doDrawFillLineV(Image* dst,int x,int y1, int y2,Color color) {
	if (y1>y2)
		swapInt(&y1,&y2);
	for (int y=y1;y<=y2;y++) {
		ImageDrawPixel(dst,x,y,color);
	}
}

// fill polygon using scan line fill algorithm (http://www.cad.zju.edu.cn/home/zhx/CG/2016/lib/exe/fetch.php?media=fillalgorithm.pdf)
void ImageFillPolygonEx(Image* dst,int* vertice_x, int * vertice_y, int num_vertice, Color color) {
	
	PolyEdgeHeap edgeTable;
	PolyEdgeList edgeList;
	PolyEdgeList  acl;
	IntList intersects;
	PolyEdgeList horizontalEdges;
	int min_y=-1,max_y=1;
	if (num_vertice<3)
		return;
	// Create Edge Table
	PolyEdgeHeap_init(&edgeTable,num_vertice);
	PolyEdgeList_init(&edgeList,num_vertice);
	
	for (int i=0;i<num_vertice;i++) {
		PPolyEdge e= (PPolyEdge)malloc(sizeof(PolyEdge));
		int i2=(i+1);
		int i0=(i-1);
		if (i2>=num_vertice) 
			i2=0;
		if (i0<0)
			i0=num_vertice-1;
		int x1=vertice_x[i];
		int y1=vertice_y[i];
		int x2=vertice_x[i2];
		int y2=vertice_y[i2];
		if (y2>y1) {
			e->max_y = y2;
			e->max_y_buddy = i2;
			e->min_y = y1;
			//			e->min_y_buddy = i0;
		} else {
			e->max_y = y1;
			e->max_y_buddy = i0;
			e->min_y = y2;
			//			e->min_y_buddy = i2;			
		}
		if (x2<x1) {
			swapInt(&x1,&x2);
			swapInt(&y1,&y2);
		}
		e->x1=x1;
		e->x2=x2;
		e->dx = x2 - x1;
		e->dy = y2 - y1;
		e->counter=0;
		if (e->dy>0) {
			e->x=e->x1;
		} else {
			e->x=e->x2;
		}
		//		int hasEdge=0;
		//		for (int i=1;i<=edgeTable.size;i++) {
		//			if (edgeTable.edges[i]->x1==e->x1
		//				&& edgeTable.edges[i]->x2==e->x2
		//				&& edgeTable.edges[i]->min_y==e->min_y
		//				&& edgeTable.edges[i]->dy == e->dy) {
		//				hasEdge=1;
		//				break;
		//			}
		//		} 
		//		if (!hasEdge)
		//			PolyEdgeHeap_insert(&edgeTable,e);
		PolyEdgeHeap_insert(&edgeTable,e);
		PolyEdgeList_append(&edgeList,e);
		if (0==i) {
			min_y = e->min_y;
			max_y = e->max_y;
		} else {
			if (e->min_y<min_y)
				min_y=e->min_y;
			if (e->max_y>max_y)
				max_y=e->max_y;
		}
		//		TraceLog(LOG_WARNING,"Edge %d: x1 %d x2 %d min_y %d max_y%d x %d max_y buddy %d",
		//			i,e->x1,e->x2,e->min_y,e->max_y,e->x,e->max_y_buddy);
	}
	
	PolyEdgeList_init(&acl,num_vertice);
	IntList_init(&intersects,num_vertice);
	PolyEdgeList_init(&horizontalEdges,num_vertice);
	for (int y=min_y;y<=max_y;y++) {
		//add edges to act;
		while(!PolyEdgeHeap_empty(&edgeTable)) {
			PPolyEdge e = PolyEdgeHeap_min(&edgeTable);
			if (e->min_y!=y)
				break;
			PolyEdgeHeap_remove_min(&edgeTable);
			PolyEdgeList_append(&acl,e);
		}
		
		//fill one horizontal line that's visible
		if (y>0 && y<dst->height) {
			PolyEdgeList_clear(&horizontalEdges);
			IntList_clear(&intersects);
			
			//sort acl by x;
			PolyEdgeList_sort_by_x(&acl);
			for (int i=0;i<acl.size;i++) {
				PPolyEdge e1 = acl.edges[i];
				if (e1->dy==0) {
					PolyEdgeList_append(&horizontalEdges,e1);
				} else if (y==e1->min_y) {
					IntList_append(&intersects, e1->x);
				} else if (y==e1->max_y) {
					PPolyEdge e2=edgeList.edges[e1->max_y_buddy];
					if (y!=e2->min_y) 
						IntList_append(&intersects, e1->x);
				} else {
					IntList_append(&intersects, e1->x);
				}
			}
			
			//			TraceLog(LOG_WARNING,"%d %d %d",y, acl.size,intersects.size);
			for (int i=0;i+1<intersects.size;i+=2) {
				//				TraceLog(LOG_WARNING,"%d, %d, %d, %d",intersects.size,i,intersects.edges[i],intersects.edges[i+1]);
				doDrawFillLineH(dst,intersects.datas[i],intersects.datas[i+1],y,color);
			}
			for (int i=0;i<horizontalEdges.size;i++) {
				PPolyEdge e=horizontalEdges.edges[i];
				doDrawFillLineH(dst,e->x1,e->x2,y,color);			
			}
		}
		//remove edges that no need; 
		//update edges' x
		int i=0;
		while (i<acl.size) {
			//remove edges that no need;
			PPolyEdge e=acl.edges[i];
			if (e->max_y==y) {
				PolyEdgeList_remove(&acl,i);
				continue;
			}
			//update edges' x
			//Bresenham’s or mid-point scan line conversion algorithm
			if (e->dx==0) {
				
			} else if (e->dy>0) {
				e->counter+=e->dx;
				while(e->counter>e->dy) {
					e->x++;
					e->counter-=e->dy;
				}
				if (e->x>e->x2)
					e->x=e->x2;
			} else if (e->dy<0) {
				e->counter-=e->dx;
				while(e->counter<e->dy){
					e->x--;
					e->counter-=e->dy;
				}
				if (e->x<e->x1)
					e->x=e->x1;
			}
			i++;
		}
	}
	
	PolyEdgeHeap_free(&edgeTable);
	PolyEdgeList_free_all_nodes(&edgeList);
	PolyEdgeList_free(&edgeList);
	PolyEdgeList_free(&acl);
	PolyEdgeList_free(&horizontalEdges);
	IntList_free(&intersects);
	
	//	FILE *fp=fopen("r:\\test.dat","w");
	//	fprintf(fp,"int num=%d;\n",num_vertice);
	//	fprintf(fp,"int vx[]={\n");
	//	for (int i=0;i<num_vertice;i++) {
	//		fprintf(fp,"\t%d",vertice_x[i]);
	//		if (i<num_vertice-1)
	//			fprintf(fp,",\n");
	//	}
	//	fprintf(fp,"};\n");
	//	fprintf(fp,"int vy[]={\n");
	//	for (int i=0;i<num_vertice;i++) {
	//		fprintf(fp,"\t%d",vertice_y[i]);
	//		if (i<num_vertice-1)
	//			fprintf(fp,",\n");
	//	}
	//	fprintf(fp,"};\n");
	//	fclose(fp);
	
}

void ImageDrawPolygonEx(Image* dst,int* vertice_x,  int * vertice_y, int num_vertice, int lineWidth, Color color) {
	for (int i=0;i<num_vertice;i++) {
		ImageDrawLineEx(dst,vertice_x[i],vertice_y[i],
			vertice_x[(i+1)%num_vertice],vertice_y[(i+1)%num_vertice],
			lineWidth,
			color
			);
	}	
}

void ImageDrawPolylineEx(Image* dst,int* points_x,  int * points_y, int num_vertice, int lineWidth, Color color) {
	for (int i=0;i<num_vertice-1;i++) {
		ImageDrawLineEx(dst,points_x[i],points_y[i],
			points_x[i+1],points_y[i+1],
			lineWidth,
			color
			);
	}	
}

void ImageFillTriangleEx(Image* dst, int x0, int y0, int x1, int y1, int x2, int y2, Color fillColor) {
	int X0,Y0,X1,Y1,X2,Y2;
	if (y0<=y1) {
		if (y2<=y0) {
			X0=x2;
			Y0=y2;
			X1=x0;
			Y1=y0;
			X2=x1;
			Y2=y1;
		} else if (y2<=y1) {
			X0=x0;
			Y0=y0;
			X1=x2;
			Y1=y2;
			X2=x1;
			Y2=y1;
		} else {
			X0=x0;
			Y0=y0;
			X1=x1;
			Y1=y1;
			X2=x2;
			Y2=y2;
		} 
	} else {
		if (y2<=y1) {
			X0=x2;
			Y0=y2;
			X1=x1;
			Y1=y1;
			X2=x0;
			Y2=y0;			
		} else if (y2<=y0) {
			X0=x1;
			Y0=y1;
			X1=x2;
			Y1=y2;
			X2=x0;
			Y2=y0;			
		} else {
			X0=x1;
			Y0=y1;
			X1=x0;
			Y1=y0;
			X2=x2;
			Y2=y2;						
		}
	}
	x0=X0;
	y0=Y0;
	x1=X1;
	y1=Y1;
	x2=X2;
	y2=Y2;
	
	//upper triangle
	int dy0 = y2-y0;
	int dx0 = x2-x0;
	int xi0;
	if (dx0<0) {
		xi0=-1;
		dx0=-dx0;
	} else {
		xi0=1;
	}
	int C0=0;
	int dy1 = y2-y1;	
	int dx1 = x2-x1;
	int xi1;
	if (dx1<0) {
		xi1=-1;
		dx1=-dx1;
	} else {
		xi1=1;
	}
	int C1=0;
	int dy2 = y1-y0;	
	int dx2 = x1-x0;
	int xi2;
	if (dx2<0) {
		xi2=-1;
		dx2=-dx2;
	} else {
		xi2=1;
	}
	int C2=0;
	int xx0,xx1,xx2;
	xx0=x0;
	xx2=x0;
	for (int y=y0;y<y1;y++) {
		doDrawFillLineH(dst,xx0,xx2,y,fillColor);
		C0+=dx0;
		while (C0>=dy0 && dy0!=0) {
			xx0+=xi0;
			C0-=dy0;
		}
		C2+=dx2;
		while (C2>=dy2 && dy2!=0) {
			xx2+=xi2;
			C2-=dy2;
		}
	}
	//lower triangle
	xx1=x1;
	for (int y=y1;y<y2;y++) {
		doDrawFillLineH(dst,xx0,xx1,y,fillColor);
		C0+=dx0;
		while (C0>=dy0 && dy0!=0) {
			xx0+=xi0;
			C0-=dy0;
		}
		C1+=dx1;
		while (C1>=dy1 && dy1!=0) {
			xx1+=xi1;
			C1-=dy1;
		}
	}
}

void ImageFillRectangleEx(Image* dst, int left, int top, int width, int height, Color fillColor) {
	int right = left+width;
	int bottom = top+height;
	if (left>right) 
		swapInt(&left,&right);
	if (top>bottom)
		swapInt(&top,&bottom);
	for (int y=top;y<=bottom;y++) {
		doDrawFillLineH(dst,left,right,y,fillColor);
	}	
}
void ImageDrawRectangleEx(Image* dst, int left, int top, int width, int height, int borderWidth, Color color) {
	int right = left+width;
	int bottom = top+height;
	if (left>right) 
		swapInt(&left,&right);
	if (top>bottom)
		swapInt(&top,&bottom);
	ImageDrawLineEx(dst,left,top,right,top,borderWidth,color);
	ImageDrawLineEx(dst,right,top,right,bottom,borderWidth,color);
	ImageDrawLineEx(dst,left,bottom,right,bottom,borderWidth,color);
	ImageDrawLineEx(dst,left,top,left,bottom,borderWidth,color);
}

void ImageFillRoundRectEx(Image* dst, int left, int top, int width, int height, int rx, int ry,  Color fillColor) {
	int right = left+width;
	int bottom = top+height;
	if (left>right) 
		swapInt(&left,&right);
	if (top>bottom)
		swapInt(&top,&bottom);
	rx=MIN(rx,(right-left)/2);
	ry=MIN(ry,(bottom-top)/2);
	//	int x0=left;
	int x1=left+rx;
	int x2=right-rx;
	//	int x3=right;
	//	int y0=top;
	int y1=top+ry;
	int y2=bottom-ry;
	//	int y3=bottom;
	int twoASquare=2*rx*rx;
	int twoBSquare=2*ry*ry;
	int x=rx;
	int y=0;
	int XChange = ry*ry*(1-2*rx);
	int YChange = rx*rx;
	int ellipseError = 0;
	int stoppingX = twoBSquare*rx;
	int stoppingY = 0;
	//first stage, y'>-1
	while (stoppingX>=stoppingY) {
		doDrawFillLineH(dst,x1-x,x2+x,y1-y,fillColor);
		doDrawFillLineH(dst,x1-x,x2+x,y2+y,fillColor);
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
	y=ry;
	XChange = ry * ry;
	YChange = rx * rx * (1-2*ry);
	ellipseError = 0;
	stoppingX = 0;
	stoppingY = twoASquare*ry;
	while (stoppingX<=stoppingY) {
		x++;
		stoppingX+=twoBSquare;
		ellipseError += XChange;
		XChange+=twoBSquare;
		if ((2*ellipseError+YChange)>0) {
			doDrawFillLineH(dst,x1-x,x2+x,y1-y,fillColor);
			doDrawFillLineH(dst,x1-x,x2+x,y2+y,fillColor);
			y-=1;
			stoppingY-=twoASquare;
			ellipseError+=YChange;
			YChange+=twoASquare;
		}				
	}	
	for (int y=y1;y<=y2;y++) {
		doDrawFillLineH(dst,left,right,y,fillColor);
	}	
	
}

void ImageDrawRoundRectEx(Image* dst, int left, int top, int width, int height, int rx, int ry, int borderWidth, Color color) {
	int right = left+width;
	int bottom = top+height;
	if (left>right) 
		swapInt(&left,&right);
	if (top>bottom)
		swapInt(&top,&bottom);
	rx=MIN(rx,(right-left)/2);
	ry=MIN(ry,(bottom-top)/2);
	int bx0=left;
	int bx1=left+rx;
	int bx2=right-rx;
	int bx3=right;
	int by0=top;
	int by1=top+ry;
	int by2=bottom-ry;
	int by3=bottom;
	int radiusX1=rx-borderWidth/2;
	int radiusX2=rx+borderWidth/2;
	int radiusY1=ry-borderWidth/2;
	int radiusY2=ry+borderWidth/2;
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
		doDrawFillLineH(dst,bx1-x2,bx1-x1,by1-y,color);
		doDrawFillLineH(dst,bx2+x1,bx2+x2,by1-y,color);
		doDrawFillLineH(dst,bx1-x2,bx1-x1,by2+y,color);
		doDrawFillLineH(dst,bx2+x1,bx2+x2,by2+y,color);
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
		doDrawFillLineV(dst,bx1-x,by1-y1,by1-y2,color);
		doDrawFillLineV(dst,bx1-x,by2+y1,by2+y2,color);
		doDrawFillLineV(dst,bx2+x,by1-y1,by1-y2,color);
		doDrawFillLineV(dst,bx2+x,by2+y1,by2+y2,color);
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
	ImageDrawLineEx(dst,bx1,by0,bx2,by0,borderWidth,color);
	ImageDrawLineEx(dst,bx1,by3,bx2,by3,borderWidth,color);
	ImageDrawLineEx(dst,bx0,by1,bx0,by2,borderWidth,color);
	ImageDrawLineEx(dst,bx3,by1,bx3,by2,borderWidth,color);
}

static float normalizedAngle(float angle) {
	float PI2 = 2*PI;
	while(angle<0) {
		angle+=PI2;
	}
	while(angle>PI2){
		angle-=PI2;
	}	
	return angle;
}

// sector (beginAngle < endAngle) between 0 to PI
static void doFillSector1(Image* dst, int cx, int cy, int radiusX, int radiusY, float beginAngle, float endAngle, Color fillColor ) {
	//arc
	int twoASquare=2*radiusX*radiusX;
	int twoBSquare=2*radiusY*radiusY;
	int xa=radiusX;
	int ya=0;
	int XChange = radiusY*radiusY*(1-2*radiusX);
	int YChange = radiusX*radiusX;
	int ellipseError = 0;
	int stoppingX = twoBSquare*radiusX;
	int stoppingY = 0;
	//	TraceLog(LOG_WARNING,"Sector1:%f %f",beginAngle,endAngle);
	//radius
	int x0=0;
	int y0=0;
	float a = radiusX;
	float b = radiusY;
	float r1=a*b/sqrt(pow(b*cos(beginAngle),2)+pow(a*sin(beginAngle),2));
	int x1=round(r1*cos(beginAngle));
	int y1=round(r1*sin(beginAngle));
	float r2=a*b/sqrt(pow(b*cos(endAngle),2)+pow(a*sin(endAngle),2));
	int x2=round(r2*cos(endAngle));
	int y2=round(r2*sin(endAngle));
	if (x1==x2 && y1==y2)
		return;
	int dy0 = y2-y0;
	int dx0 = x2-x0;
	int xi0;
	if (dx0<0) {
		xi0=-1;
		dx0=-dx0;
	} else {
		xi0=1;
	}
	int C0=0;
	int dy1 = y1-y0;	
	int dx1 = x1-x0;
	int xi1;
	if (dx1<0) {
		xi1=-1;
		dx1=-dx1;
	} else {
		xi1=1;
	}
	//	TraceLog(LOG_WARNING," -- %f %f %d %d %d %d",beginAngle,endAngle,dx0,dy0,dx1,dy1);
	int C1=0;
	int xx0,xx1;
	xx0=x0;
	xx1=x0;
	if (dy0==0) 
		xx0=x2;
	if (dy1==0)
		xx1=x1;
	//first stage, y'>-1
	while (stoppingX>=stoppingY) {
		int tx0,tx1;
		if (-xa<xx0)
			tx0=xx0;
		else
			tx0=-xa;
		if (xa>xx1)
			tx1=xx1;
		else
			tx1=xa;
		if (tx0<tx1)
			doDrawFillLineH(dst,cx+tx0,cx+tx1,cy-ya,fillColor);
		//		TraceLog(LOG_WARNING,"State 1: %d,%d,%d,%d,%d,%d",tx0,tx1,xx0,xx1,-xa,xa);
		ya+=1;
		stoppingY += twoASquare;
		ellipseError += YChange;
		YChange += twoASquare;
		if ((2*ellipseError+XChange)>0){
			xa-=1;
			stoppingX -= twoBSquare;
			ellipseError += XChange;
			XChange+=twoBSquare;
		}
		if (ya<=y2 && dy0!=0) {
			C0+=dx0;
			while (C0>=dy0) {
				xx0+=xi0;
				C0-=dy0;
			}
		}
		if (ya<=y1 && dy1!=0) {
			C1+=dx1;
			while (C1>=dy1) {
				xx1+=xi1;
				C1-=dy1;
			}
		}
	}	
	//second stage, y'<-1
	xa=0;
	ya=radiusY;
	XChange = radiusY * radiusY;
	YChange = radiusX * radiusX * (1-2*radiusY);
	ellipseError = 0;
	stoppingX = 0;
	stoppingY = twoASquare*radiusY;
	bool bothSide = (xi0<0 && xi1<0) || (xi0>0 && xi1>0) ;
	xx0=x2;
	xx1=x1;
	while (stoppingX<=stoppingY) {
		xa++;
		stoppingX+=twoBSquare;
		ellipseError += XChange;
		XChange+=twoBSquare;
		if ((2*ellipseError+YChange)>0) {
			int tx0,tx1;
			if (-xa<xx0)
				tx0=xx0;
			else
				tx0=-xa;
			if (xa>xx1)
				tx1=xx1;
			else
				tx1=xa;
			if (! (bothSide && (ya>y2 && ya>y1))) {
				//				if (tx0<tx1)
				doDrawFillLineH(dst,cx+tx0,cx+tx1,cy-ya,fillColor);	
				//				TraceLog(LOG_WARNING,"State 2: %d,%d,%d,%d,%d,%d",tx0,tx1,xx0,xx1,-xa,xa);				
			}			
			ya-=1;
			stoppingY-=twoASquare;
			ellipseError+=YChange;
			YChange+=twoASquare;
			if (ya<=y2 && dy0!=0) {
				C0+=dx0;
				while (C0>=dy0) {
					xx0-=xi0;
					C0-=dy0;
				}				
			}
			if (ya<=y1 && dy1!=0) {
				C1+=dx1;
				while (C1>=dy1) {
					xx1-=xi1;
					C1-=dy1;
				}				
			}			
		}				
	}
}

// sector (beginAngle < endAngle) between PI to 2PI
static void doFillSector2(Image* dst, int cx, int cy, int radiusX,int radiusY, float beginAngle, float endAngle, Color fillColor ) {
	//arc
	int twoASquare=2*radiusX*radiusX;
	int twoBSquare=2*radiusY*radiusY;
	int xa=radiusX;
	int ya=0;
	int XChange = radiusY*radiusY*(1-2*radiusX);
	int YChange = radiusX*radiusX;
	int ellipseError = 0;
	int stoppingX = twoBSquare*radiusX;
	int stoppingY = 0;
	float temp1 = 2*PI-endAngle;
	float temp2 = 2*PI-beginAngle;
	beginAngle = temp1;
	endAngle = temp2;
	//	TraceLog(LOG_WARNING,"Sector2:%f %f",beginAngle,endAngle);
	//radius
	int x0=0;
	int y0=0;
	float a = radiusX;
	float b = radiusY;
	float r1=a*b/sqrt(pow(b*cos(beginAngle),2)+pow(a*sin(beginAngle),2));
	int x1=round(r1*cos(beginAngle));
	int y1=round(r1*sin(beginAngle));
	float r2=a*b/sqrt(pow(b*cos(endAngle),2)+pow(a*sin(endAngle),2));
	int x2=round(r2*cos(endAngle));
	int y2=round(r2*sin(endAngle));
	if (x1==x2 && y1==y2)
		return;
	
	int dy0 = y2-y0;
	int dx0 = x2-x0;
	int xi0;
	if (dx0<0) {
		xi0=-1;
		dx0=-dx0;
	} else {
		xi0=1;
	}
	int C0=0;
	int dy1 = y1-y0;	
	int dx1 = x1-x0;
	int xi1;
	if (dx1<0) {
		xi1=-1;
		dx1=-dx1;
	} else {
		xi1=1;
	}
	//	TraceLog(LOG_WARNING," -- %f %f %d %d %d %d",beginAngle,endAngle,dx0,dy0,dx1,dy1);
	int C1=0;
	int xx0,xx1;
	xx0=x0;
	xx1=x0;
	if (dy0==0) 
		xx0=x2;
	if (dy1==0)
		xx1=x1;
	//first stage, y'>-1
	while (stoppingX>=stoppingY) {
		int tx0,tx1;
		if (-xa<xx0)
			tx0=xx0;
		else
			tx0=-xa;
		if (xa>xx1)
			tx1=xx1;
		else
			tx1=xa;
		if (tx0<tx1)
			doDrawFillLineH(dst,cx+tx0,cx+tx1,cy+ya,fillColor);
		//		TraceLog(LOG_WARNING,"State 1: %d,%d,%d,%d,%d,%d",tx0,tx1,xx0,xx1,-xa,xa);
		ya+=1;
		stoppingY += twoASquare;
		ellipseError += YChange;
		YChange += twoASquare;
		if ((2*ellipseError+XChange)>0){
			xa-=1;
			stoppingX -= twoBSquare;
			ellipseError += XChange;
			XChange+=twoBSquare;
		}
		if (ya<=y2 && dy0!=0) {
			C0+=dx0;
			while (C0>=dy0) {
				xx0+=xi0;
				C0-=dy0;
			}
		}
		if (ya<=y1 && dy1!=0) {
			C1+=dx1;
			while (C1>=dy1) {
				xx1+=xi1;
				C1-=dy1;
			}
		}
	}	
	//second stage, y'<-1
	xa=0;
	ya=radiusY;
	XChange = radiusY * radiusY;
	YChange = radiusX * radiusX * (1-2*radiusY);
	ellipseError = 0;
	stoppingX = 0;
	stoppingY = twoASquare*radiusY;
	bool bothSide = (xi0<0 && xi1<0) || (xi0>0 && xi1>0) ;
	xx0=x2;
	xx1=x1;
	while (stoppingX<=stoppingY) {
		xa++;
		stoppingX+=twoBSquare;
		ellipseError += XChange;
		XChange+=twoBSquare;
		if ((2*ellipseError+YChange)>0) {
			int tx0,tx1;
			if (-xa<xx0)
				tx0=xx0;
			else
				tx0=-xa;
			if (xa>xx1)
				tx1=xx1;
			else
				tx1=xa;
			if (! (bothSide && (ya>y2 && ya>y1))) {
				//				if (tx0<tx1)
				doDrawFillLineH(dst,cx+tx0,cx+tx1,cy+ya,fillColor);	
				//				TraceLog(LOG_WARNING,"State 2: %d,%d,%d,%d,%d,%d",tx0,tx1,xx0,xx1,-xa,xa);				
			}			
			ya-=1;
			stoppingY-=twoASquare;
			ellipseError+=YChange;
			YChange+=twoASquare;
			if (ya<=y2 && dy0!=0) {
				C0+=dx0;
				while (C0>=dy0) {
					xx0-=xi0;
					C0-=dy0;
				}				
			}
			if (ya<=y1 && dy1!=0) {
				C1+=dx1;
				while (C1>=dy1) {
					xx1-=xi1;
					C1-=dy1;
				}				
			}			
		}				
	}
}

void ImageFillSectorEx(Image* dst,int cx, int cy, int radiusX, int radiusY, float beginAngle, float endAngle, Color fillColor) {
	float totalAngle=endAngle-beginAngle;
	if (totalAngle>=2*PI) {
		ImageFillEllipseEx(dst,cx,cy,radiusX, radiusY,fillColor);
		return;
	}
	if (totalAngle>0) {
		beginAngle=normalizedAngle(beginAngle);
		endAngle=normalizedAngle(beginAngle+totalAngle);
	} else {
		beginAngle=normalizedAngle(endAngle);
		endAngle=normalizedAngle(beginAngle-totalAngle);
	}
	if (endAngle>=beginAngle) {
		if (endAngle<=PI) {
			doFillSector1(dst,cx,cy,radiusX,radiusY, beginAngle,endAngle,fillColor);
		} else if (beginAngle>=PI) {
			doFillSector2(dst,cx,cy,radiusX,radiusY, beginAngle,endAngle,fillColor);			
		} else {
			doFillSector1(dst,cx,cy,radiusX,radiusY,beginAngle,PI,fillColor);						
			doFillSector2(dst,cx,cy,radiusX,radiusY,PI,endAngle,fillColor);						
		}
	} else {
		if (beginAngle<=PI) {
			doFillSector1(dst,cx,cy,radiusX,radiusY,beginAngle,PI,fillColor);						
			doFillSector1(dst,cx,cy,radiusX,radiusY,0,endAngle,fillColor);						
			doFillSector2(dst,cx,cy,radiusX,radiusY,PI,2*PI,fillColor);						
		} else if (endAngle<=PI){
			doFillSector1(dst,cx,cy,radiusX,radiusY,0,endAngle,fillColor);									
			doFillSector2(dst,cx,cy,radiusX,radiusY,beginAngle,2*PI,fillColor);									
		} else {
			doFillSector1(dst,cx,cy,radiusX,radiusY,0,PI,fillColor);									
			doFillSector2(dst,cx,cy,radiusX,radiusY,beginAngle,2*PI,fillColor);									
			doFillSector2(dst,cx,cy,radiusX,radiusY,PI,endAngle,fillColor);
		}
	}
}

static void doDrawArc1(Image* dst,int cx, int cy, int radiusX,int radiusY, float beginAngle, float endAngle,  int lineWidth, Color color) {
	int radiusX1=radiusX-lineWidth/2;
	int radiusX2=radiusX+lineWidth/2;
	int radiusY1=radiusY-lineWidth/2;
	int radiusY2=radiusY+lineWidth/2;
	int twoASquare1=2*radiusX1*radiusX1;
	int twoBSquare1=2*radiusY1*radiusY1;
	int twoASquare2=2*radiusX2*radiusX2;
	int twoBSquare2=2*radiusY2*radiusY2;
	int xa1=radiusX1;
	int xa2=radiusX2;
	int ya=0;	
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
	
	int x0=0;
	int y0=0;
	float a = radiusX2;
	float b = radiusY2;
	float r1=a*b/sqrt(pow(b*cos(beginAngle),2)+pow(a*sin(beginAngle),2));
	int x1=round(r1*cos(beginAngle));
	int y1=round(r1*sin(beginAngle));
	float r2=a*b/sqrt(pow(b*cos(endAngle),2)+pow(a*sin(endAngle),2));
	int x2=round(r2*cos(endAngle));
	int y2=round(r2*sin(endAngle));
	if (x1==x2 && y1==y2)
		return;
	int dy0 = y2-y0;
	int dx0 = x2-x0;
	int xi0;
	if (dx0<0) {
		xi0=-1;
		dx0=-dx0;
	} else {
		xi0=1;
	}
	int C0=0;
	int dy1 = y1-y0;	
	int dx1 = x1-x0;
	int xi1;
	if (dx1<0) {
		xi1=-1;
		dx1=-dx1;
	} else {
		xi1=1;
	}
	//	TraceLog(LOG_WARNING," -- %f %f %d %d %d %d",beginAngle,endAngle,dx0,dy0,dx1,dy1);
	int C1=0;
	int xx0,xx1;
	xx0=x0;
	xx1=x0;
	if (dy0==0) 
		xx0=x2;
	if (dy1==0)
		xx1=x1;
	
	//first stage, y'>-1
	IntList endpoints;
	IntList_init(&endpoints,4);
	while (stoppingX2>=stoppingY2) {
		IntList_clear(&endpoints);
		if (-xa2>=xx0 && -xa2<=xx1) {
			IntList_append(&endpoints,-xa2);
		} 
		if (-xa1>=xx0 && -xa1<=xx1) {
			IntList_append(&endpoints,-xa1);
		} 
		if (xa1>=xx0 && xa1<=xx1) {
			IntList_append(&endpoints,xa1);
		} 
		if (xa2>=xx0 && xa2<=xx1) {
			IntList_append(&endpoints,xa2);
		} 
		if (-xa2<xx0 && xx0<=-xa1)
			IntList_append(&endpoints,xx0);
		else if (xa1<xx0 && xx0<=xa2)
			IntList_append(&endpoints,xx0);
		if (-xa2<=xx1 && xx1<-xa1)
			IntList_append(&endpoints,xx1);
		else if (xa1<=xx1 && xx1<xa2)
			IntList_append(&endpoints,xx1);
		IntList_sort(&endpoints);
		for (int i=0;i+1<endpoints.size;i+=2) {
			doDrawFillLineH(dst, cx+endpoints.datas[i],cx+endpoints.datas[i+1], cy-ya, color);
		}
		ya+=1;
		if (stoppingX1>=stoppingY1) {
			stoppingY1 += twoASquare1;
			ellipseError1 += YChange1;
			YChange1 += twoASquare1;
			if ((2*ellipseError1+XChange1)>0){
				xa1-=1;
				stoppingX1 -= twoBSquare1;
				ellipseError1 += XChange1;
				XChange1+=twoBSquare1;
			}
		}
		stoppingY2 += twoASquare2;
		ellipseError2 += YChange2;
		YChange2 += twoASquare2;
		if ((2*ellipseError2+XChange2)>0){
			xa2-=1;
			stoppingX2 -= twoBSquare2;
			ellipseError2 += XChange2;
			XChange2+=twoBSquare2;
		}
		if (dy0!=0) {
			C0+=dx0;
			while (C0>=dy0) {
				xx0+=xi0;
				C0-=dy0;
			}
		}
		if (dy1!=0) {
			C1+=dx1;
			while (C1>=dy1) {
				xx1+=xi1;
				C1-=dy1;
			}
		}
	}
	//second stage, y'<-1
	int xa=0;
	int ya1=radiusY1;
	int ya2=radiusY2;
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
	
	int yy0,yy1;
	C0=0;
	C1=0;
	yy0=y0;
	yy1=y0;
	if (dx0==0) 
		yy0=radiusY2;
	if (dx1==0)
		yy1=radiusY2;
	bool l0_at_left = (dx0==0) || xi0>0;
	bool l1_at_left = (dx1==0) || xi1>0;
	while (stoppingX2<=stoppingY2) {
		IntList_clear(&endpoints);
		if (l0_at_left ) {
			if (ya1>=yy1 && ya1<=yy0) {
				IntList_append(&endpoints,ya1);
			} 
			if (ya2>=yy1 && ya2<=yy0) {
				IntList_append(&endpoints,ya2);
			} 
			if (ya1<yy0 && yy0<=ya2)
				IntList_append(&endpoints,yy0);
			if (ya1<=yy1 && yy1<ya2)
				IntList_append(&endpoints,yy1);
			IntList_sort(&endpoints);
			for (int i=0;i+1<endpoints.size;i+=2) {
				doDrawFillLineV(dst, cx+xa,cy-endpoints.datas[i],cy-endpoints.datas[i+1], color);
			}			
		} else {
			if (!l1_at_left) {
				if (ya1>=yy0 && ya1<=yy1) {
					IntList_append(&endpoints,ya1);
				} 
				if (ya2>=yy0 && ya2<=yy1) {
					IntList_append(&endpoints,ya2);
				} 
				if (ya1<=yy0 && yy0<ya2)
					IntList_append(&endpoints,yy0);
				if (ya1<yy1 && yy1<=ya2)
					IntList_append(&endpoints,yy1);
				IntList_sort(&endpoints);
				for (int i=0;i+1<endpoints.size;i+=2) {
					doDrawFillLineV(dst, cx-xa,cy-endpoints.datas[i],cy-endpoints.datas[i+1], color);
				} 
			} else {
				IntList_clear(&endpoints);
				if (ya1>=yy0)
					IntList_append(&endpoints,ya1);
				if (ya2>=yy0)
					IntList_append(&endpoints,ya2);
				if (ya1<=yy0 && yy0<ya2)
					IntList_append(&endpoints,yy0);
				IntList_sort(&endpoints);
				for (int i=0;i+1<endpoints.size;i+=2) {
					doDrawFillLineV(dst, cx-xa,cy-endpoints.datas[i],cy-endpoints.datas[i+1], color);
				}
				IntList_clear(&endpoints);
				if (ya1>=yy1)
					IntList_append(&endpoints,ya1);
				if (ya2>=yy1)
					IntList_append(&endpoints,ya2);
				if (ya1<=yy1 && yy1<ya2)
					IntList_append(&endpoints,yy1);
				IntList_sort(&endpoints);
				for (int i=0;i+1<endpoints.size;i+=2) {
					doDrawFillLineV(dst, cx+xa,cy-endpoints.datas[i],cy-endpoints.datas[i+1], color);
				} 
			}			
		} 
		
		//		TraceLog(LOG_WARNING,"%d %d %d %d %d %d",endpoints.size(),xa,ya1,ya2,yy0,yy1);
		xa++;
		if (stoppingX1<=stoppingY1) {
			stoppingX1+=twoBSquare1;
			ellipseError1 += XChange1;
			XChange1+=twoBSquare1;
			if ((2*ellipseError1+YChange1)>0) {
				ya1-=1;
				stoppingY1-=twoASquare1;
				ellipseError1+=YChange1;
				YChange1+=twoASquare1;
			}							
		}
		stoppingX2+=twoBSquare2;
		ellipseError2 += XChange2;
		XChange2+=twoBSquare2;
		if ((2*ellipseError2+YChange2)>0) {
			ya2-=1;
			stoppingY2-=twoASquare2;
			ellipseError2+=YChange2;
			YChange2+=twoASquare2;
		}				
		if (dx0!=0&&dy0!=0) {
			C0+=dy0;
			while (C0>=dx0) {
				yy0+=1;
				C0-=dx0;
			}
		}
		if (dx1!=0&&dy1!=0) {
			C1+=dy1;
			while (C1>=dx1) {
				yy1+=1;
				C1-=dx1;
			}
		}
	}
	IntList_free(&endpoints);
}

static void doDrawArc2(Image* dst,int cx, int cy, int radiusX,int radiusY, float beginAngle, float endAngle,  int lineWidth, Color color) {
	int radiusX1=radiusX-lineWidth/2;
	int radiusX2=radiusX+lineWidth/2;
	int radiusY1=radiusY-lineWidth/2;
	int radiusY2=radiusY+lineWidth/2;
	int twoASquare1=2*radiusX1*radiusX1;
	int twoBSquare1=2*radiusY1*radiusY1;
	int twoASquare2=2*radiusX2*radiusX2;
	int twoBSquare2=2*radiusY2*radiusY2;
	int xa1=radiusX1;
	int xa2=radiusX2;
	int ya=0;	
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
	
	float temp1 = 2*PI-endAngle;
	float temp2 = 2*PI-beginAngle;
	beginAngle = temp1;
	endAngle = temp2;
	int x0=0;
	int y0=0;
	float a = radiusX2;
	float b = radiusY2;
	float r1=a*b/sqrt(pow(b*cos(beginAngle),2)+pow(a*sin(beginAngle),2));
	int x1=round(r1*cos(beginAngle));
	int y1=round(r1*sin(beginAngle));
	float r2=a*b/sqrt(pow(b*cos(endAngle),2)+pow(a*sin(endAngle),2));
	int x2=round(r2*cos(endAngle));
	int y2=round(r2*sin(endAngle));
	if (x1==x2 && y1==y2)
		return;
	int dy0 = y2-y0;
	int dx0 = x2-x0;
	int xi0;
	if (dx0<0) {
		xi0=-1;
		dx0=-dx0;
	} else {
		xi0=1;
	}
	int C0=0;
	int dy1 = y1-y0;	
	int dx1 = x1-x0;
	int xi1;
	if (dx1<0) {
		xi1=-1;
		dx1=-dx1;
	} else {
		xi1=1;
	}
	//	TraceLog(LOG_WARNING," -- %f %f %d %d %d %d",beginAngle,endAngle,dx0,dy0,dx1,dy1);
	int C1=0;
	int xx0,xx1;
	xx0=x0;
	xx1=x0;
	if (dy0==0) 
		xx0=x2;
	if (dy1==0)
		xx1=x1;
	
	//first stage, y'>-1
	IntList endpoints;
	IntList_init(&endpoints,4);
	while (stoppingX2>=stoppingY2) {
		IntList_clear(&endpoints);
		if (-xa2>=xx0 && -xa2<=xx1) {
			IntList_append(&endpoints,-xa2);
		} 
		if (-xa1>=xx0 && -xa1<=xx1) {
			IntList_append(&endpoints,-xa1);
		} 
		if (xa1>=xx0 && xa1<=xx1) {
			IntList_append(&endpoints,xa1);
		} 
		if (xa2>=xx0 && xa2<=xx1) {
			IntList_append(&endpoints,xa2);
		} 
		if (-xa2<xx0 && xx0<=-xa1)
			IntList_append(&endpoints,xx0);
		else if (xa1<xx0 && xx0<=xa2)
			IntList_append(&endpoints,xx0);
		if (-xa2<=xx1 && xx1<-xa1)
			IntList_append(&endpoints,xx1);
		else if (xa1<=xx1 && xx1<xa2)
			IntList_append(&endpoints,xx1);
		IntList_sort(&endpoints);
		for (int i=0;i+1<endpoints.size;i+=2) {
			doDrawFillLineH(dst, cx+endpoints.datas[i],cx+endpoints.datas[i+1], cy+ya, color);
		}
		ya+=1;
		if (stoppingX1>=stoppingY1) {
			stoppingY1 += twoASquare1;
			ellipseError1 += YChange1;
			YChange1 += twoASquare1;
			if ((2*ellipseError1+XChange1)>0){
				xa1-=1;
				stoppingX1 -= twoBSquare1;
				ellipseError1 += XChange1;
				XChange1+=twoBSquare1;
			}
		}
		stoppingY2 += twoASquare2;
		ellipseError2 += YChange2;
		YChange2 += twoASquare2;
		if ((2*ellipseError2+XChange2)>0){
			xa2-=1;
			stoppingX2 -= twoBSquare2;
			ellipseError2 += XChange2;
			XChange2+=twoBSquare2;
		}
		if (dy0!=0) {
			C0+=dx0;
			while (C0>=dy0) {
				xx0+=xi0;
				C0-=dy0;
			}
		}
		if (dy1!=0) {
			C1+=dx1;
			while (C1>=dy1) {
				xx1+=xi1;
				C1-=dy1;
			}
		}
	}
	//second stage, y'<-1
	int xa=0;
	int ya1=radiusY1;
	int ya2=radiusY2;
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
	
	int yy0,yy1;
	C0=0;
	C1=0;
	yy0=y0;
	yy1=y0;
	if (dx0==0) 
		yy0=radiusY2;
	if (dx1==0)
		yy1=radiusY2;
	bool l0_at_left = (dx0==0) || xi0>0;
	bool l1_at_left = (dx1==0) || xi1>0;
	while (stoppingX2<=stoppingY2) {
		IntList_clear(&endpoints);
		if (l0_at_left ) {
			if (ya1>=yy1 && ya1<=yy0) {
				IntList_append(&endpoints,ya1);
			} 
			if (ya2>=yy1 && ya2<=yy0) {
				IntList_append(&endpoints,ya2);
			} 
			if (ya1<yy0 && yy0<=ya2)
				IntList_append(&endpoints,yy0);
			if (ya1<=yy1 && yy1<ya2)
				IntList_append(&endpoints,yy1);	
			IntList_sort(&endpoints);
			for (int i=0;i+1<endpoints.size;i+=2) {
				doDrawFillLineV(dst, cx+xa,cy+endpoints.datas[i],cy+endpoints.datas[i+1], color);
			}			
		} else {
			if (!l1_at_left) {
				if (ya1>=yy0 && ya1<=yy1) {
					IntList_append(&endpoints,ya1);
				} 
				if (ya2>=yy0 && ya2<=yy1) {
					IntList_append(&endpoints,ya2);
				} 
				if (ya1<=yy0 && yy0<ya2)
					IntList_append(&endpoints,yy0);
				if (ya1<yy1 && yy1<=ya2)
					IntList_append(&endpoints,yy1);
				IntList_sort(&endpoints);
				for (int i=0;i+1<endpoints.size;i+=2) {
					doDrawFillLineV(dst, cx-xa,cy+endpoints.datas[i],cy+endpoints.datas[i+1], color);
				} 
			} else {
				IntList_clear(&endpoints);
				if (ya1>=yy0)
					IntList_append(&endpoints,ya1);
				if (ya2>=yy0)
					IntList_append(&endpoints,ya2);
				if (ya1<=yy0 && yy0<ya2)
					IntList_append(&endpoints,yy0);
				IntList_sort(&endpoints);
				for (int i=0;i+1<endpoints.size;i+=2) {
					doDrawFillLineV(dst, cx-xa,cy+endpoints.datas[i],cy+endpoints.datas[i+1], color);
				} 
				IntList_clear(&endpoints);
				if (ya1>=yy1)
					IntList_append(&endpoints,ya1);
				if (ya2>=yy1)
					IntList_append(&endpoints,ya2);
				if (ya1<=yy1 && yy1<ya2)
					IntList_append(&endpoints,yy1);
				IntList_sort(&endpoints);
				for (int i=0;i+1<endpoints.size;i+=2) {
					doDrawFillLineV(dst, cx+xa,cy+endpoints.datas[i],cy+endpoints.datas[i+1], color);
				} 
			}			
		} 
		
		//		TraceLog(LOG_WARNING,"%d %d %d %d %d %d",endpoints.size(),xa,ya1,ya2,yy0,yy1);
		xa++;
		if (stoppingX1<=stoppingY1) {
			stoppingX1+=twoBSquare1;
			ellipseError1 += XChange1;
			XChange1+=twoBSquare1;
			if ((2*ellipseError1+YChange1)>0) {
				ya1-=1;
				stoppingY1-=twoASquare1;
				ellipseError1+=YChange1;
				YChange1+=twoASquare1;
			}							
		}
		stoppingX2+=twoBSquare2;
		ellipseError2 += XChange2;
		XChange2+=twoBSquare2;
		if ((2*ellipseError2+YChange2)>0) {
			ya2-=1;
			stoppingY2-=twoASquare2;
			ellipseError2+=YChange2;
			YChange2+=twoASquare2;
		}				
		if (dx0!=0&&dy0!=0) {
			C0+=dy0;
			while (C0>=dx0) {
				yy0+=1;
				C0-=dx0;
			}
		}
		if (dx1!=0&&dy1!=0) {
			C1+=dy1;
			while (C1>=dx1) {
				yy1+=1;
				C1-=dx1;
			}
		}
	}		
	IntList_free(&endpoints);
}

void ImageDrawCubicBezierEx(Image* dst, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int lineWidth, Color color){
	int max_x = MAX(x0,x1);
	max_x = MAX(max_x,x2);
	max_x = MAX(max_x,x3);
	int max_y = MAX(y0,y1);
	max_y = MAX(max_y,y2);
	max_y = MAX(max_y,y3);
	int min_x = MIN(x0,x1);
	min_x = MIN(min_x,x2);
	min_x = MIN(min_x,x3);
	int min_y = MIN(y0,y1);
	min_y = MIN(min_y,y2);
	min_y = MIN(min_y,y3);
	int maxd=(max_x-min_x)+(max_y-min_y);
	if (maxd==0)
		maxd=1;
	int oldX=x0;
	int oldY=y0;
	for (int i=1;i<maxd;i++) {
		double t = (double)i/maxd;
		double t1 = 1-t;
		int x = round(t1*t1*t1*x0+3*t*t1*t1*x1+3*t*t*t1*x2+t*t*t*x3);
		int y = round(t1*t1*t1*y0+3*t*t1*t1*y1+3*t*t*t1*y2+t*t*t*y3);
		int dx=x-oldX;
		int dy=y-oldY;
		if (dx!=0 || dy!=0) {
			ImageDrawLineEx(dst,oldX,oldY,x,y,lineWidth,color);
		}
		oldX=x;
		oldY=y;
	}
	int dx=x3-oldX;
	int dy=y3-oldY;
	if (dx!=0 || dy!=0) {
		ImageDrawLineEx(dst,oldX,oldY,x3,y3,lineWidth,color);
	}
}


void ImageDrawArcEx(Image* dst,int cx, int cy, int radiusX,int radiusY, float beginAngle, float endAngle,  int lineWidth, Color color) {
	float totalAngle=endAngle-beginAngle;
	if (totalAngle>=2*PI) {
		ImageDrawEllipseEx(dst,cx,cy,radiusX, radiusY,lineWidth, color);
		return;
	}
	if (totalAngle>0) {
		beginAngle=normalizedAngle(beginAngle);
		endAngle=normalizedAngle(beginAngle+totalAngle);
	} else {
		beginAngle=normalizedAngle(endAngle);
		endAngle=normalizedAngle(beginAngle-totalAngle);
	}
	if (endAngle>=beginAngle) {
		if (endAngle<=PI) {
			doDrawArc1(dst,cx,cy,radiusX,radiusY, beginAngle,endAngle,lineWidth, color);
		} else if (beginAngle>=PI) {
			doDrawArc2(dst,cx,cy,radiusX,radiusY, beginAngle,endAngle,lineWidth, color);			
		} else {
			doDrawArc1(dst,cx,cy,radiusX,radiusY,beginAngle,PI,lineWidth, color);						
			doDrawArc2(dst,cx,cy,radiusX,radiusY,PI,endAngle,lineWidth, color);						
		}
	} else {
		if (beginAngle<=PI) {
			doDrawArc1(dst,cx,cy,radiusX,radiusY,beginAngle,PI,lineWidth, color);						
			doDrawArc1(dst,cx,cy,radiusX,radiusY,0,endAngle,lineWidth, color);						
			doDrawArc2(dst,cx,cy,radiusX,radiusY,PI,2*PI,lineWidth, color);						
		} else if (endAngle<=PI){
			doDrawArc1(dst,cx,cy,radiusX,radiusY,0,endAngle,lineWidth, color);									
			doDrawArc2(dst,cx,cy,radiusX,radiusY,beginAngle,2*PI,lineWidth, color);									
		} else {
			doDrawArc1(dst,cx,cy,radiusX,radiusY,0,PI,lineWidth, color);									
			doDrawArc2(dst,cx,cy,radiusX,radiusY,beginAngle,2*PI,lineWidth, color);									
			doDrawArc2(dst,cx,cy,radiusX,radiusY,PI,endAngle,lineWidth, color);
		}
	}
	if (_default_image_join_style==IMAGE_JOIN_ROUND) {
		float a = radiusX;
		float b = radiusY;
		float r1=a*b/sqrt(pow(b*cos(beginAngle),2)+pow(a*sin(beginAngle),2));
		int x1=cx+round(r1*cos(beginAngle));
		int y1=cy-round(r1*sin(beginAngle));
		float r2=a*b/sqrt(pow(b*cos(endAngle),2)+pow(a*sin(endAngle),2));
		int x2=cx+round(r2*cos(endAngle));
		int y2=cy-round(r2*sin(endAngle));
		ImageDrawPointEx(dst,x2,y2,lineWidth,color);
		ImageDrawPointEx(dst,x1,y1,lineWidth,color);
	}
}

void ImageDrawSectorEx(Image* dst,int cx, int cy, int radiusX,int radiusY, float beginAngle, float endAngle, int lineWidth, Color color) {
	ImageDrawArcEx(dst,cx,cy,radiusX,radiusY,beginAngle,endAngle,lineWidth,color);
	float a = radiusX;
	float b = radiusY;
	float r1=a*b/sqrt(pow(b*cos(beginAngle),2)+pow(a*sin(beginAngle),2));
	int x1=round(r1*cos(beginAngle));
	int y1=round(r1*sin(beginAngle));
	float r2=a*b/sqrt(pow(b*cos(endAngle),2)+pow(a*sin(endAngle),2));
	int x2=round(r2*cos(endAngle));
	int y2=round(r2*sin(endAngle));
	ImageDrawLineEx(dst,cx,cy,cx+x1,cy-y1,lineWidth,color);
	if (x1!=x2 || y1!=y2)
		ImageDrawLineEx(dst,cx,cy,cx+x2,cy-y2,lineWidth,color);
}

void ImageSetJoinStyle(ImageJoinStyle style) {
	_default_image_join_style = style;
}

ImageJoinStyle ImageGetJoinStyle() {
	return _default_image_join_style;
}
