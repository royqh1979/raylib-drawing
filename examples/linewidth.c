#include "../src/rdrawing.h"
#include <time.h>

void genLine(Image* img) {
	int x1,x2,y1,y2;
	x1=GetRandomValue(200,600);
	y1=GetRandomValue(100,500);
	x2=GetRandomValue(200,600);
	y2=GetRandomValue(100,500);
	ImageClearBackground(img,WHITE);
	ImageDrawPointEx(img,x1,y1,50,DARKCYAN);
	ImageDrawPointEx(img,x2,y2,50,DARKCYAN);
	ImageDrawLineEx(img,x1,y1,x2,y2,50,BLACK);	
}

int main() {
	InitWindow(800,600,"test");
	
	SetTraceLogLevel(LOG_WARNING);
	
	Image img=GenImageColor(800,600,WHITE);
	
	SetRandomSeed(time(NULL));

	genLine(&img);
	SetTargetFPS(120);
	while (!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			genLine(&img);
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
