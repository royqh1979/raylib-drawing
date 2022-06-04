#include "rdrawing.h"
#include <math.h>
#include <algorithm>
#include <queue>
#include <string.h>
#include <vector>
#include <map>
#include <memory>

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
	int D_delta1=2*(dy-dx);
	int D_delta2=2*dy;
	int y=y0;
	float lwv = lineWidth*sqrt(dx*dx+dy*dy)/dx;
	float b = lineWidth*dy / dx;
	float end_dx=b*lineWidth/2/lwv;
	float end_dy=b*b/2/lwv;
	int end_mid_x0 = x0+end_dy;
	int end_mid_x1 = x1-end_dy;
	int	w = round(lwv/2.0);
	for (int x=x0;x<=x1;x++) {
		if (x>=end_mid_x0 && x<=end_mid_x1) {
			for (int yy=y-w;yy<=y+w;yy++) {
				ImageDrawPixel(image,
					x,yy,color);
			}
		}
		if (D>0) {
			y+=yi;
			D+=D_delta1;
		}else{
			D+=D_delta2;
		}
	}
	if (yi>0) {
		int vx[3];
		int vy[3];
		vx[0]=end_mid_x0;
		vy[0]=y0+end_dy-w;
		vx[1]=end_mid_x0;
		vy[1]=y0+end_dy+w;
		vx[2]=x0-end_dx;
		vy[2]=y0-end_dy+w;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
		vx[0]=end_mid_x1;
		vy[0]=y1-end_dy-w;
		vx[1]=end_mid_x1;
		vy[1]=y1-end_dy+w;
		vx[2]=x1+end_dx;
		vy[2]=y1+end_dy-w;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
	} else {
		int vx[3];
		int vy[3];
		vx[0]=end_mid_x0;
		vy[0]=y0-end_dy-w;
		vx[1]=end_mid_x0;
		vy[1]=y0-end_dy+w;
		vx[2]=x0-end_dx;
		vy[2]=y0+end_dy-w;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
		vx[0]=end_mid_x1;
		vy[0]=y1+end_dy-w;
		vx[1]=end_mid_x1;
		vy[1]=y1+end_dy+w;
		vx[2]=x1+end_dx;
		vy[2]=y1-end_dy+w;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
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
	int D_delta1=2*(dx-dy);
	int D_delta2=2*dx;
	
	int x = x0;
	float lwh = lineWidth*sqrt(dx*dx+dy*dy)/dy;
	float b = lineWidth*dx / dy;
	float end_dy=b*lineWidth/2/lwh;
	float end_dx=b*b/2/lwh;
	int end_mid_y0 = y0+end_dy;
	int end_mid_y1 = y1-end_dy;
	int	w = round(lwh/2.0);
	
	//	TraceLog(LOG_WARNING, "%f, %f, %f,%d,%d,%d,%d\n", lwh,b,end_dy, y0,y1,end_mid_y0,end_mid_y1);
	for (int y=y0;y<=y1;y++) {
		if (y>=end_mid_y0 && y<=end_mid_y1) {
			for (int xx=x-w;xx<=x+w;xx++) {
				ImageDrawPixel(image,
					xx,y,color);
			}
		}
		if (D>0) {
			x+=xi;
			D+=D_delta1;
		}else{
			D+=D_delta2;
		}
	}
	if (xi>0) {
		int vx[3];
		int vy[3];
		vx[0]=x0+end_dx-w;
		vy[0]=end_mid_y0;
		vx[1]=x0+end_dx+w;
		vy[1]=end_mid_y0;
		vx[2]=x0-end_dx+w;
		vy[2]=y0-end_dy;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
		vx[0]=x1-end_dx-w;
		vy[0]=end_mid_y1;
		vx[1]=x1-end_dx+w;
		vy[1]=end_mid_y1;
		vx[2]=x1+end_dx-w;
		vy[2]=y1+end_dy;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
	} else {
		int vx[3];
		int vy[3];
		vx[0]=x0-end_dx-w;
		vy[0]=end_mid_y0;
		vx[1]=x0-end_dx+w;
		vy[1]=end_mid_y0;
		vx[2]=x0+end_dx-w;
		vy[2]=y0-end_dy;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
		vx[0]=x1+end_dx-w;
		vy[0]=end_mid_y1;
		vx[1]=x1+end_dx+w;
		vy[1]=end_mid_y1;
		vx[2]=x1-end_dx+w;
		vy[2]=y1+end_dy;
		ImageFillTriangleEx(image,vx[0],vy[0],vx[1],vy[1],vx[2],vy[2],color);
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


struct PolyEdge {
	int x1;			// x1 is always <= x2
	int x2;
	int x;			// x at the current scanline
	int min_y;
	int max_y;
	int dx;
	int dy;
	int counter;    // use it to update x
	//	int min_y_buddy; //don't need
	int max_y_buddy; 
};

using PPolyEdge = std::shared_ptr<PolyEdge>;

static void doDrawFillLineH(Image* dst,int x1,int x2, int y,Color color) {
	if (x1>x2)
		std::swap(x1,x2);
	for (int x=x1;x<=x2;x++) {
		ImageDrawPixel(dst,x,y,color);
	}
}

static bool sortPolyEdgeByx(PPolyEdge e1,PPolyEdge e2) {
	return e1->x<e2->x;
}

// fill polygon using scan line fill algorithm (http://www.cad.zju.edu.cn/home/zhx/CG/2016/lib/exe/fetch.php?media=fillalgorithm.pdf)
void ImageFillPolygonEx(Image* dst,int* vertice_x, int * vertice_y, int num_vertice, Color color) {
	
	// Create Edge Table
	std::multimap<int,PPolyEdge> edgeTable;
	std::vector<PPolyEdge> edgeList;
	int min_y=-1,max_y=1;
	for (int i=0;i<num_vertice;i++) {
		PPolyEdge e=std::make_shared<PolyEdge>();
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
			std::swap(x1,x2);
			std::swap(y1,y2);
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
		edgeTable.insert(std::pair<int,PPolyEdge>{e->min_y,e});
		edgeList.push_back(e);
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
	std::vector<PPolyEdge> acl;
	std::vector<int> intersects;
	std::vector<PPolyEdge> horizontalEdges;
	for (int y=min_y;y<=max_y;y++) {
		intersects.clear();
		horizontalEdges.clear();
		//add edges to act;
		auto edges = edgeTable.equal_range(y);
		if (edges.first!=std::end(edgeTable)) {
			for (auto i=edges.first;i!=edges.second; i++) {
				PPolyEdge e = (*i).second;
				acl.push_back(e);	
			}
		}
		//sort acl by x;
		std::sort(acl.begin(),acl.end(),sortPolyEdgeByx);
		for (int i=0;i<acl.size();i++) {
			PPolyEdge e1 = acl[i];
			
			if (e1->dy==0) {
				horizontalEdges.push_back(e1);
			} else if (y==e1->min_y) { 
				intersects.push_back(e1->x);
			} else if (y==e1->max_y) {
				PPolyEdge e2=edgeList[e1->max_y_buddy];
				if (y!=e2->min_y) 
					intersects.push_back(e1->x);
			} else {
				intersects.push_back(e1->x);
			}
		}
		//remove edges that no need; 
		//update edges' x
		auto it=acl.begin();
		while (it!=acl.end()) {
			//remove edges that no need; 
			if ((*it)->max_y==y) {
				it=acl.erase(it);
				continue;
			}
			//update edges' x
			//Bresenham’s or mid-point scan line conversion algorithm
			PPolyEdge e = (*it);
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
			it++;
		}
		
		for (int i=0;i+1<intersects.size();i+=2) {
			//			TraceLog(LOG_WARNING,"%d, %d, %d, %d",intersects.size(),i,intersects[i],intersects[i+1]);
			doDrawFillLineH(dst,intersects[i],intersects[i+1],y,color);
		}
		for (PPolyEdge e:horizontalEdges) {
			doDrawFillLineH(dst,e->x1,e->x2,y,color);			
		}
	}	
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
		while (C0>=dy0) {
			xx0+=xi0;
			C0-=dy0;
		}
		C2+=dx2;
		while (C2>=dy2) {
			xx2+=xi2;
			C2-=dy2;
		}
	}
	xx1=x1;
	for (int y=y1;y<y2;y++) {
		doDrawFillLineH(dst,xx0,xx1,y,fillColor);
		C0+=dx0;
		while (C0>=dy0) {
			xx0+=xi0;
			C0-=dy0;
		}
		C1+=dx1;
		while (C1>=dy1) {
			xx1+=xi1;
			C1-=dy1;
		}
	}
}

