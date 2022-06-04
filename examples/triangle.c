#include "../src/rdrawing.h"
#include <time.h>

void genTriangle(Image* img) {
	int x[3];
	int y[3];
	x[0]=GetRandomValue(200,600);
	x[1]=GetRandomValue(200,600);
	x[2]=GetRandomValue(200,600);
	y[0]=GetRandomValue(100,500);
	y[1]=GetRandomValue(100,500);
	y[2]=GetRandomValue(100,500);
	ImageClearBackground(img,WHITE);
	
	ImageFillTriangleEx(img,x[0],y[0],x[1],y[1],x[2],y[2],LIGHTRED);
	ImageDrawPolygonEx(img,x,y,3,1,BLACK);	
}
int main() {
	InitWindow(800,600,"test");
	
	SetTraceLogLevel(LOG_WARNING);
	
	Image img=GenImageColor(800,600,WHITE);
	
	//	int x1,x2,y1,y2;
	//	x1=300;y1=100;
	//	x2=100;y2=290;
	//	ImageDrawPointEx(&img,x1,y1,50,DARKCYAN);
	//	ImageDrawPointEx(&img,x2,y2,50,DARKCYAN);
	//	ImageDrawLineEx(&img,x1,y1,x2,y2,50,BLACK);
	SetRandomSeed(time(NULL));
	
	genTriangle(&img);
	SetTargetFPS(120);
	int num=0;
	while (!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			genTriangle(&img);
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
