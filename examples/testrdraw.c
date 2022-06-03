#include "../src/rdrawing.h"
#include <time.h>

int main() {
	InitWindow(800,600,"test");
	
	SetTraceLogLevel(LOG_WARNING);
	
	Image img=GenImageColor(800,600,WHITE);
	
	int x1,x2,y1,y2;
	x1=300;y1=100;
	x2=100;y2=290;
	ImageDrawPointEx(&img,x1,y1,50,DARKCYAN);
	ImageDrawPointEx(&img,x2,y2,50,DARKCYAN);
	ImageDrawLineEx(&img,x1,y1,x2,y2,50,BLACK);
	
	SetTargetFPS(120);
	int num=0;
	while (!WindowShouldClose()) {
		Texture2D texture = LoadTextureFromImage(img);
		BeginDrawing();
		DrawTexture(texture,0,0,WHITE);
		
		EndDrawing();
		UnloadTexture(texture);
	}
	UnloadImage(img);
	return 0;
}
