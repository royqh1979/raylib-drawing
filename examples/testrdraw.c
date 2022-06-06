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
	InitWindow(650,650,"test");
	
	SetTraceLogLevel(LOG_WARNING);
	
	Image img=GenImageColor(650,650,WHITE);
	
	SetRandomSeed(time(NULL));

	for (int i=0;i<=12;i++) {
		for (int j=0;j<=12;j++) {
			ImageFillSectorEx(&img,i*50+25,j*50+25,24,i*PI/6,j*PI/6,DARKCYAN);
			ImageDrawLine(&img,i*50+25,j*50+25,i*50+50,j*50+25,BLACK);
		}
	}

//	ImageFillSectorEx(&img,300,300,200,PI/3,PI/6,DARKCYAN);
//	ImageFillEllipseEx(&img,400,300,250,50,DARKRED);

	//genLine(&img);
	SetTargetFPS(120);
	int num=0;
	while (!WindowShouldClose()) {
//		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
//			genLine(&img);
//		}
		Texture2D texture = LoadTextureFromImage(img);
		BeginDrawing();
		DrawTexture(texture,0,0,WHITE);
		
		EndDrawing();
		UnloadTexture(texture);
	}
	UnloadImage(img);
	return 0;
}
