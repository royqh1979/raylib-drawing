#include "../src/rdrawing.h"
#include <time.h>

int main() {
	InitWindow(800,600,"test");
	
	Image img=GenImageColor(800,600,WHITE);
	
	//ImageDrawCircleEx(&img, 400,300,200,10,BLACK);
	SetRandomSeed(time(NULL));
	SetTraceLogLevel(LOG_WARNING);
	int vx[200];
	int vy[200];
	int size=sizeof(vx)/sizeof(int);
	//	for (int i=0;i<size;i++) {
	//		vx[i]=GetRandomValue(0,800);
	//		vy[i]=GetRandomValue(0,600);
	//	}
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
