#include "../src/rdrawing.h"
#include <time.h>

int main() {
	InitWindow(800,600,"test");
	
	Image img=GenImageColor(800,600,WHITE);
	
	//ImageDrawCircleEx(&img, 400,300,200,10,BLACK);
	SetRandomSeed(time(NULL));
	SetTraceLogLevel(LOG_WARNING);
	SetTargetFPS(120);
	int num=9;
	int vx[]={
		250,
		329,
		435,
		405,
		529,
		480,
		648,
		481,
		348};
	int vy[]={
		290,
		89,
		133,
		227,
		190,
		11,
		151,
		362,
		197};

	ImageFillPolygonEx(&img,vx,vy,num,LIGHTRED);
	ImageDrawPolygonEx(&img,vx,vy,num,3,BLACK);
	for (int i=0;i<num;i++) {
		ImageDrawPointEx(&img,vx[i],vy[i],5,BLUE);				
	}
	Texture2D texture = LoadTextureFromImage(img);
	while (!WindowShouldClose()) {
		BeginDrawing();
		DrawTexture(texture,0,0,WHITE);
		
		EndDrawing();

	}
	UnloadTexture(texture);
	UnloadImage(img);
	return 0;
}
