//#include "../src/rdrawing.h"
//#include <time.h>
//
//int main() {
//	InitWindow(800,600,"test");
//	
//	SetTraceLogLevel(LOG_WARNING);
//	
//	Image img=GenImageColor(800,600,WHITE);
//	
//	SetRandomSeed(time(NULL));
//
//	for (int i=0;i<=12;i++) {
//		for (int j=0;j<=12;j++) {
//			ImageFillSectorEx(&img,j*50+25,i*50+25,16,24,i*PI/6,j*PI/6,DARKCYAN);
//			ImageDrawArcEx(&img,j*50+25,i*50+25,16,24,i*PI/6,j*PI/6,3,DARKRED);
//			ImageFillSectorEx(&img,650+j*50+25,i*50+25,16,24,i*PI/6,-j*PI/6,DARKCYAN);
//			ImageDrawArcEx(&img,650+j*50+25,i*50+25,16,24,i*PI/6,-j*PI/6,3,DARKRED);
//		}
//	}
//
////	ImageFillSectorEx(&img,300,300,200,200,2*PI/3,5*PI/6,LIGHTRED);
////	ImageDrawArcEx(&img,300,300,200,200,0,0,5,DARKCYAN);
////	ImageFillSectorEx(&img,300,300,200,PI/3,PI/6,DARKCYAN);
////	ImageFillEllipseEx(&img,400,300,250,50,DARKRED);
//
//	//genLine(&img);
//	int center_x, center_y; //圆心坐标
//	int radius; //圆半径
//	int offset; // 同一半径各组扇形之间的角度偏移量
//	int total_offset; // 不同半径之间的角度偏移量
//	SetTargetFPS(1);
//	int num=0;
//	while (!WindowShouldClose()) {
//
//		for (center_x = 100; center_x < 800; center_x += 200) {
//			for (center_y = 100; center_y < 600; center_y += 200) {
//				total_offset = 0;
//				float h = GetRandomValue(0,180);
//				// 用色调1生成的颜色1
//				Color color1 = ColorFromHSV(h, 0.9, 0.8);
//				// 用色调2生成的颜色2
//				Color color2 = ColorFromHSV(h + 180, 0.9, 0.8);
//				for (radius = 100; radius > 0; radius = radius - 20) {
//					//半径从大到小绘制
//					for (int i = 0; i < 20; i++) {
//						//绕着旋转一周绘制扇形区域
//						
//						//各组扇形之间偏移的角度
//						offset = i * 180 / 10 + total_offset ;
//						
//						ImageFillSectorEx(&img,center_x,center_y, radius,radius, offset * DEG2RAD,
//							(2 * 180 / 60 + offset)*DEG2RAD,color1);
//						
//						ImageFillSectorEx(&img,center_x,center_y, radius,radius, (2 * 180 / 60 + offset) * DEG2RAD,
//							(3 * 180 / 60 + offset)*DEG2RAD,WHITE);
//
//						
//						ImageFillSectorEx(&img,center_x,center_y, radius,radius, (3 * 180 / 60 + offset) * DEG2RAD,
//							(5 * 180 / 60 + offset)*DEG2RAD,color2);
//						
//						ImageFillSectorEx(&img,center_x,center_y, radius,radius, (5 * 180 / 60 + offset) * DEG2RAD,
//							(6 * 180 / 60 + offset)*DEG2RAD,BLACK);
//					}
//					total_offset += 180 / 20; //不同半径间角度偏移量为180/20
//				}
//			}
//		}
//		Texture2D texture = LoadTextureFromImage(img);
//		BeginDrawing();
//		DrawTexture(texture,0,0,WHITE);
//		
//		EndDrawing();
//		UnloadTexture(texture);
//	}
//	UnloadImage(img);
//	return 0;
//}

#include "../src/rdrawing.h"
#include <time.h>

int main() {
	InitWindow(1200,600,"test");
	
	SetTraceLogLevel(LOG_WARNING);
	
	Image img=GenImageColor(1200,600,WHITE);
	
	SetRandomSeed(time(NULL));
	
	for (int i=0;i<=12;i++) {
		for (int j=0;j<=12;j++) {
			ImageFillSectorEx(&img,j*50+25,i*50+25,16,24,i*PI/6,j*PI/6,DARKCYAN);
			ImageDrawSectorEx(&img,j*50+25,i*50+25,16,24,i*PI/6,j*PI/6,3,DARKRED);
			ImageFillSectorEx(&img,650+j*50+25,i*50+25,16,24,i*PI/6,-j*PI/6,DARKCYAN);
			ImageDrawSectorEx(&img,650+j*50+25,i*50+25,16,24,i*PI/6,-j*PI/6,3,DARKRED);
		}
	}
	
//		ImageFillSectorEx(&img,300,300,200,200,PI/2,PI/2,LIGHTRED);
		//ImageDrawArcEx(&img,300,300,200,200,PI/2,PI/2,5,DARKCYAN);
	//	ImageFillSectorEx(&img,300,300,200,PI/3,PI/6,DARKCYAN);
	//	ImageFillEllipseEx(&img,400,300,250,50,DARKRED);
	
	Texture2D texture = LoadTextureFromImage(img);
	SetTargetFPS(1);
	while (!WindowShouldClose()) {
		BeginDrawing();
		DrawTexture(texture,0,0,WHITE);
		
		EndDrawing();
	}
	UnloadTexture(texture);
	UnloadImage(img);
	return 0;
}
