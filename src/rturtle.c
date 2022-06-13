#include "utils.h"
#include "rturtle.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define BASE_STEP 1

#define COLOR_EQUAL(col1, col2) ((col1.r == col2.r) && (col1.g == col2.g) && (col1.b == col2.b) && (col1.a == col2.a))

typedef struct
{
	double x;
	double y;
	double orient; //头指向角度
	bool is_pen_down;
	Color pen_color;
	int pen_size;
	int move_speed;
	Texture2D icon;
	double icon_width;
	double icon_height;
	bool is_show; //是否显示海龟
} Turtle;

typedef struct
{
	int width;
	int height;
	Color back_color;
	bool is_rewind;
	bool is_immediate;
	Image world_image;
	Texture2D background;
	bool use_background_image;
	int origin_x;
	int origin_y;
	int frame_count;
	bool window_should_close;
	bool use_grid;
	bool hide_grid;
	bool hide_background_image;
	Texture2D grids;
} World;

static World* pWorld = NULL;
static Turtle* pTurtle = NULL;

static double d2a(double degree)
{
	return degree*M_PI/180.0;
}

static void captureScreen() {
	static int count=0;
	char filename[200];
	do {
		sprintf(filename,"save_%04d.png",count);
		count++;
	} while(FileExists(filename));
	Image image = LoadImageFromScreen();
	ExportImage(image,filename);
	char msg[200];
	sprintf(msg,"Saved as %s",filename);
	ImageDrawText(&image,msg,pWorld->width-10-GetFontDefault().baseSize*strlen(msg)*1.5,
		pWorld->height-20-GetFontDefault().baseSize*2,20,pTurtle->pen_color);
	Texture2D texture=LoadTextureFromImage(image);
	BeginDrawing();
	DrawTexture(texture,0,0,pWorld->back_color);
	EndDrawing();
	UnloadImage(image);
	sleep(1);
}

static void displayWorld() {
	if (WindowShouldClose()) {
		pWorld->window_should_close = true;
	}
	Texture2D world_texture = LoadTextureFromImage(pWorld->world_image);
	
	BeginDrawing();
	ClearBackground(pWorld->back_color);
	if (pWorld->use_background_image && !pWorld->hide_background_image) {
		DrawTexture(pWorld->background,0,0,pWorld->back_color);
	}
	if (pWorld->use_grid && !pWorld->hide_grid) {
		DrawTexture(pWorld->grids,0,0,pWorld->back_color);			
	}
	DrawTexture(world_texture,0,0,pWorld->back_color);	
	if (pTurtle->is_show) {
		Vector2 pos;
		pos.x=pTurtle->x-(pTurtle->icon_width*sqrt(2)/2)*cos(d2a(pTurtle->orient+90+45));
		pos.y=pTurtle->y-(pTurtle->icon_height*sqrt(2)/2)*sin(d2a(pTurtle->orient+90+45));
		DrawTextureEx(pTurtle->icon,pos,pTurtle->orient+90,1,pWorld->back_color);
	}
	if (pWorld->use_grid && !pWorld->hide_grid) {
		char buffer[200];
		sprintf(buffer,"(%d,%d)",(int)(round(getX())),(int)(round(getY())));
		DrawText(buffer,pWorld->width-10-GetFontDefault().baseSize*strlen(buffer),
			pWorld->height-10-GetFontDefault().baseSize,20,pTurtle->pen_color);
	}
	EndDrawing();
	UnloadTexture(world_texture);
}

static void interactWithUser() {
	if (IsKeyPressed(KEY_F2)) {
		pWorld->hide_grid=!pWorld->hide_grid;
	} 
	if (IsKeyPressed(KEY_F3)) {
		pTurtle->is_show=!pTurtle->is_show; 
	} 
	if (IsKeyPressed(KEY_F5)) {
		captureScreen(); 
	} 
}
static void refreshWorld()
{
	interactWithUser();
	pWorld->frame_count++;
	pWorld->frame_count%=pTurtle->move_speed;
	if (pTurtle->move_speed>1000)
	{
		int s=pTurtle->move_speed/100;
		if (pWorld->frame_count%s!=1)
		{
			return; //skip frame
		}
		usleep(1000);
	}
	else
	{
		usleep(500000/pTurtle->move_speed);
	}
	displayWorld();
}

static void prepareTurtleOriginIcon()
{
	int width,height;
	width=32;
	height=32;
	Image icon = GenImageColor(64,64,BLANK);
	ImageDrawPixel(&icon,9,14,CLITERAL(Color){ 156, 155, 167, 96 });
	ImageDrawPixel(&icon,9,15,CLITERAL(Color){ 102, 114, 83, 198 });
	ImageDrawPixel(&icon,9,16,CLITERAL(Color){ 47, 62, 7, 242 });
	ImageDrawPixel(&icon,9,17,CLITERAL(Color){ 69, 83, 32, 232 });
	ImageDrawPixel(&icon,9,18,CLITERAL(Color){ 121, 130, 104, 161 });
	ImageDrawPixel(&icon,9,19,CLITERAL(Color){ 159, 157, 173, 92 });
	ImageDrawPixel(&icon,9,20,CLITERAL(Color){ 243, 236, 255, 3 });
	ImageDrawPixel(&icon,10,13,CLITERAL(Color){ 97, 101, 90, 177 });
	ImageDrawPixel(&icon,10,14,CLITERAL(Color){ 23, 54, 0, 255 });
	ImageDrawPixel(&icon,10,15,CLITERAL(Color){ 58, 94, 0, 255 });
	ImageDrawPixel(&icon,10,16,CLITERAL(Color){ 100, 137, 0, 255 });
	ImageDrawPixel(&icon,10,17,CLITERAL(Color){ 85, 122, 0, 255 });
	ImageDrawPixel(&icon,10,18,CLITERAL(Color){ 51, 87, 0, 255 });
	ImageDrawPixel(&icon,10,19,CLITERAL(Color){ 32, 60, 0, 255 });
	ImageDrawPixel(&icon,10,20,CLITERAL(Color){ 64, 78, 47, 220 });
	ImageDrawPixel(&icon,10,21,CLITERAL(Color){ 156, 154, 172, 96 });
	ImageDrawPixel(&icon,11,12,CLITERAL(Color){ 127, 130, 127, 133 });
	ImageDrawPixel(&icon,11,13,CLITERAL(Color){ 21, 54, 0, 255 });
	ImageDrawPixel(&icon,11,14,CLITERAL(Color){ 100, 137, 0, 255 });
	ImageDrawPixel(&icon,11,15,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,11,16,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,11,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,11,18,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,11,19,CLITERAL(Color){ 100, 135, 0, 255 });
	ImageDrawPixel(&icon,11,20,CLITERAL(Color){ 68, 107, 0, 255 });
	ImageDrawPixel(&icon,11,21,CLITERAL(Color){ 28, 56, 0, 255 });
	ImageDrawPixel(&icon,11,22,CLITERAL(Color){ 92, 98, 94, 175 });
	ImageDrawPixel(&icon,11,23,CLITERAL(Color){ 242, 235, 255, 9 });
	ImageDrawPixel(&icon,12,12,CLITERAL(Color){ 74, 90, 32, 227 });
	ImageDrawPixel(&icon,12,13,CLITERAL(Color){ 74, 113, 0, 255 });
	ImageDrawPixel(&icon,12,14,CLITERAL(Color){ 89, 119, 0, 255 });
	ImageDrawPixel(&icon,12,15,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,12,16,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,12,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,12,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,12,19,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,12,20,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,12,21,CLITERAL(Color){ 98, 135, 0, 255 });
	ImageDrawPixel(&icon,12,22,CLITERAL(Color){ 41, 77, 0, 255 });
	ImageDrawPixel(&icon,12,23,CLITERAL(Color){ 57, 68, 49, 218 });
	ImageDrawPixel(&icon,12,24,CLITERAL(Color){ 211, 205, 224, 43 });
	ImageDrawPixel(&icon,12,54,CLITERAL(Color){ 187, 182, 205, 59 });
	ImageDrawPixel(&icon,12,55,CLITERAL(Color){ 131, 136, 129, 135 });
	ImageDrawPixel(&icon,12,56,CLITERAL(Color){ 113, 125, 86, 178 });
	ImageDrawPixel(&icon,12,57,CLITERAL(Color){ 109, 121, 81, 179 });
	ImageDrawPixel(&icon,12,58,CLITERAL(Color){ 127, 130, 135, 130 });
	ImageDrawPixel(&icon,12,59,CLITERAL(Color){ 231, 226, 240, 24 });
	ImageDrawPixel(&icon,13,12,CLITERAL(Color){ 81, 95, 49, 211 });
	ImageDrawPixel(&icon,13,13,CLITERAL(Color){ 61, 99, 0, 255 });
	ImageDrawPixel(&icon,13,14,CLITERAL(Color){ 92, 123, 0, 255 });
	ImageDrawPixel(&icon,13,15,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,13,16,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,13,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,13,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,13,19,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,13,20,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,13,21,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,13,22,CLITERAL(Color){ 101, 134, 0, 255 });
	ImageDrawPixel(&icon,13,23,CLITERAL(Color){ 56, 94, 0, 255 });
	ImageDrawPixel(&icon,13,24,CLITERAL(Color){ 38, 56, 16, 251 });
	ImageDrawPixel(&icon,13,25,CLITERAL(Color){ 190, 185, 202, 67 });
	ImageDrawPixel(&icon,13,30,CLITERAL(Color){ 223, 243, 252, 8 });
	ImageDrawPixel(&icon,13,31,CLITERAL(Color){ 191, 205, 211, 49 });
	ImageDrawPixel(&icon,13,32,CLITERAL(Color){ 162, 165, 166, 93 });
	ImageDrawPixel(&icon,13,33,CLITERAL(Color){ 139, 134, 132, 135 });
	ImageDrawPixel(&icon,13,34,CLITERAL(Color){ 130, 117, 112, 153 });
	ImageDrawPixel(&icon,13,35,CLITERAL(Color){ 119, 102, 95, 168 });
	ImageDrawPixel(&icon,13,36,CLITERAL(Color){ 116, 98, 91, 168 });
	ImageDrawPixel(&icon,13,37,CLITERAL(Color){ 125, 111, 105, 156 });
	ImageDrawPixel(&icon,13,38,CLITERAL(Color){ 134, 127, 123, 140 });
	ImageDrawPixel(&icon,13,39,CLITERAL(Color){ 142, 144, 143, 105 });
	ImageDrawPixel(&icon,13,40,CLITERAL(Color){ 183, 194, 198, 58 });
	ImageDrawPixel(&icon,13,41,CLITERAL(Color){ 215, 235, 243, 18 });
	ImageDrawPixel(&icon,13,52,CLITERAL(Color){ 200, 194, 214, 52 });
	ImageDrawPixel(&icon,13,53,CLITERAL(Color){ 86, 95, 83, 183 });
	ImageDrawPixel(&icon,13,54,CLITERAL(Color){ 40, 64, 0, 255 });
	ImageDrawPixel(&icon,13,55,CLITERAL(Color){ 40, 72, 0, 255 });
	ImageDrawPixel(&icon,13,56,CLITERAL(Color){ 60, 97, 0, 255 });
	ImageDrawPixel(&icon,13,57,CLITERAL(Color){ 61, 98, 0, 255 });
	ImageDrawPixel(&icon,13,58,CLITERAL(Color){ 31, 64, 0, 255 });
	ImageDrawPixel(&icon,13,59,CLITERAL(Color){ 43, 58, 27, 240 });
	ImageDrawPixel(&icon,13,60,CLITERAL(Color){ 200, 196, 212, 54 });
	ImageDrawPixel(&icon,14,12,CLITERAL(Color){ 158, 156, 165, 93 });
	ImageDrawPixel(&icon,14,13,CLITERAL(Color){ 17, 45, 0, 255 });
	ImageDrawPixel(&icon,14,14,CLITERAL(Color){ 91, 130, 0, 255 });
	ImageDrawPixel(&icon,14,15,CLITERAL(Color){ 94, 125, 0, 255 });
	ImageDrawPixel(&icon,14,16,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,14,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,14,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,14,19,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,14,20,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,14,21,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,14,22,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,14,23,CLITERAL(Color){ 98, 130, 0, 255 });
	ImageDrawPixel(&icon,14,24,CLITERAL(Color){ 70, 108, 0, 255 });
	ImageDrawPixel(&icon,14,25,CLITERAL(Color){ 41, 64, 0, 255 });
	ImageDrawPixel(&icon,14,26,CLITERAL(Color){ 179, 192, 194, 75 });
	ImageDrawPixel(&icon,14,27,CLITERAL(Color){ 217, 238, 252, 12 });
	ImageDrawPixel(&icon,14,28,CLITERAL(Color){ 153, 162, 166, 101 });
	ImageDrawPixel(&icon,14,29,CLITERAL(Color){ 112, 91, 84, 180 });
	ImageDrawPixel(&icon,14,30,CLITERAL(Color){ 110, 54, 31, 232 });
	ImageDrawPixel(&icon,14,31,CLITERAL(Color){ 115, 30, 0, 255 });
	ImageDrawPixel(&icon,14,32,CLITERAL(Color){ 120, 16, 0, 255 });
	ImageDrawPixel(&icon,14,33,CLITERAL(Color){ 123, 6, 0, 255 });
	ImageDrawPixel(&icon,14,34,CLITERAL(Color){ 133, 8, 0, 255 });
	ImageDrawPixel(&icon,14,35,CLITERAL(Color){ 142, 12, 0, 255 });
	ImageDrawPixel(&icon,14,36,CLITERAL(Color){ 143, 13, 0, 255 });
	ImageDrawPixel(&icon,14,37,CLITERAL(Color){ 135, 8, 0, 255 });
	ImageDrawPixel(&icon,14,38,CLITERAL(Color){ 126, 5, 0, 255 });
	ImageDrawPixel(&icon,14,39,CLITERAL(Color){ 125, 16, 0, 255 });
	ImageDrawPixel(&icon,14,40,CLITERAL(Color){ 119, 28, 0, 255 });
	ImageDrawPixel(&icon,14,41,CLITERAL(Color){ 111, 46, 20, 243 });
	ImageDrawPixel(&icon,14,42,CLITERAL(Color){ 107, 78, 68, 194 });
	ImageDrawPixel(&icon,14,43,CLITERAL(Color){ 137, 139, 142, 123 });
	ImageDrawPixel(&icon,14,44,CLITERAL(Color){ 195, 218, 227, 34 });
	ImageDrawPixel(&icon,14,51,CLITERAL(Color){ 138, 137, 150, 119 });
	ImageDrawPixel(&icon,14,52,CLITERAL(Color){ 33, 55, 0, 255 });
	ImageDrawPixel(&icon,14,53,CLITERAL(Color){ 49, 86, 0, 255 });
	ImageDrawPixel(&icon,14,54,CLITERAL(Color){ 95, 132, 0, 255 });
	ImageDrawPixel(&icon,14,55,CLITERAL(Color){ 95, 128, 0, 255 });
	ImageDrawPixel(&icon,14,56,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,14,57,CLITERAL(Color){ 89, 119, 0, 255 });
	ImageDrawPixel(&icon,14,58,CLITERAL(Color){ 102, 136, 0, 255 });
	ImageDrawPixel(&icon,14,59,CLITERAL(Color){ 53, 93, 0, 255 });
	ImageDrawPixel(&icon,14,60,CLITERAL(Color){ 63, 77, 30, 241 });
	ImageDrawPixel(&icon,14,61,CLITERAL(Color){ 239, 235, 248, 11 });
	ImageDrawPixel(&icon,15,13,CLITERAL(Color){ 133, 133, 132, 134 });
	ImageDrawPixel(&icon,15,14,CLITERAL(Color){ 24, 49, 0, 255 });
	ImageDrawPixel(&icon,15,15,CLITERAL(Color){ 58, 96, 0, 255 });
	ImageDrawPixel(&icon,15,16,CLITERAL(Color){ 95, 132, 0, 255 });
	ImageDrawPixel(&icon,15,17,CLITERAL(Color){ 94, 127, 0, 255 });
	ImageDrawPixel(&icon,15,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,19,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,20,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,21,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,22,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,15,23,CLITERAL(Color){ 95, 125, 0, 255 });
	ImageDrawPixel(&icon,15,24,CLITERAL(Color){ 98, 147, 0, 255 });
	ImageDrawPixel(&icon,15,25,CLITERAL(Color){ 41, 104, 0, 255 });
	ImageDrawPixel(&icon,15,26,CLITERAL(Color){ 31, 35, 8, 255 });
	ImageDrawPixel(&icon,15,27,CLITERAL(Color){ 102, 42, 32, 237 });
	ImageDrawPixel(&icon,15,28,CLITERAL(Color){ 112, 10, 0, 255 });
	ImageDrawPixel(&icon,15,29,CLITERAL(Color){ 151, 13, 0, 255 });
	ImageDrawPixel(&icon,15,30,CLITERAL(Color){ 178, 34, 0, 255 });
	ImageDrawPixel(&icon,15,31,CLITERAL(Color){ 165, 40, 0, 255 });
	ImageDrawPixel(&icon,15,32,CLITERAL(Color){ 150, 42, 0, 255 });
	ImageDrawPixel(&icon,15,33,CLITERAL(Color){ 138, 42, 3, 255 });
	ImageDrawPixel(&icon,15,34,CLITERAL(Color){ 127, 40, 4, 255 });
	ImageDrawPixel(&icon,15,35,CLITERAL(Color){ 115, 35, 4, 255 });
	ImageDrawPixel(&icon,15,36,CLITERAL(Color){ 113, 35, 4, 255 });
	ImageDrawPixel(&icon,15,37,CLITERAL(Color){ 123, 39, 5, 255 });
	ImageDrawPixel(&icon,15,38,CLITERAL(Color){ 134, 41, 4, 255 });
	ImageDrawPixel(&icon,15,39,CLITERAL(Color){ 146, 42, 1, 255 });
	ImageDrawPixel(&icon,15,40,CLITERAL(Color){ 158, 39, 0, 255 });
	ImageDrawPixel(&icon,15,41,CLITERAL(Color){ 174, 36, 0, 255 });
	ImageDrawPixel(&icon,15,42,CLITERAL(Color){ 161, 20, 0, 255 });
	ImageDrawPixel(&icon,15,43,CLITERAL(Color){ 120, 5, 0, 255 });
	ImageDrawPixel(&icon,15,44,CLITERAL(Color){ 95, 29, 6, 255 });
	ImageDrawPixel(&icon,15,45,CLITERAL(Color){ 100, 86, 86, 180 });
	ImageDrawPixel(&icon,15,46,CLITERAL(Color){ 169, 191, 201, 65 });
	ImageDrawPixel(&icon,15,50,CLITERAL(Color){ 96, 100, 101, 167 });
	ImageDrawPixel(&icon,15,51,CLITERAL(Color){ 31, 61, 0, 255 });
	ImageDrawPixel(&icon,15,52,CLITERAL(Color){ 84, 123, 0, 255 });
	ImageDrawPixel(&icon,15,53,CLITERAL(Color){ 96, 128, 0, 255 });
	ImageDrawPixel(&icon,15,54,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,15,55,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,56,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,57,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,15,58,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,15,59,CLITERAL(Color){ 100, 137, 0, 255 });
	ImageDrawPixel(&icon,15,60,CLITERAL(Color){ 44, 73, 0, 255 });
	ImageDrawPixel(&icon,15,61,CLITERAL(Color){ 187, 184, 202, 57 });
	ImageDrawPixel(&icon,16,14,CLITERAL(Color){ 175, 170, 188, 75 });
	ImageDrawPixel(&icon,16,15,CLITERAL(Color){ 76, 88, 66, 200 });
	ImageDrawPixel(&icon,16,16,CLITERAL(Color){ 45, 70, 0, 255 });
	ImageDrawPixel(&icon,16,17,CLITERAL(Color){ 44, 77, 0, 255 });
	ImageDrawPixel(&icon,16,18,CLITERAL(Color){ 79, 117, 0, 255 });
	ImageDrawPixel(&icon,16,19,CLITERAL(Color){ 97, 131, 0, 255 });
	ImageDrawPixel(&icon,16,20,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,16,21,CLITERAL(Color){ 88, 117, 0, 255 });
	ImageDrawPixel(&icon,16,22,CLITERAL(Color){ 98, 135, 0, 255 });
	ImageDrawPixel(&icon,16,23,CLITERAL(Color){ 68, 126, 2, 255 });
	ImageDrawPixel(&icon,16,24,CLITERAL(Color){ 47, 73, 6, 255 });
	ImageDrawPixel(&icon,16,25,CLITERAL(Color){ 93, 36, 7, 255 });
	ImageDrawPixel(&icon,16,26,CLITERAL(Color){ 160, 30, 0, 255 });
	ImageDrawPixel(&icon,16,27,CLITERAL(Color){ 187, 37, 0, 255 });
	ImageDrawPixel(&icon,16,28,CLITERAL(Color){ 163, 46, 0, 255 });
	ImageDrawPixel(&icon,16,29,CLITERAL(Color){ 123, 40, 5, 255 });
	ImageDrawPixel(&icon,16,30,CLITERAL(Color){ 92, 32, 6, 255 });
	ImageDrawPixel(&icon,16,31,CLITERAL(Color){ 74, 27, 9, 255 });
	ImageDrawPixel(&icon,16,32,CLITERAL(Color){ 74, 29, 11, 255 });
	ImageDrawPixel(&icon,16,33,CLITERAL(Color){ 74, 30, 12, 255 });
	ImageDrawPixel(&icon,16,34,CLITERAL(Color){ 77, 31, 12, 255 });
	ImageDrawPixel(&icon,16,35,CLITERAL(Color){ 51, 22, 11, 255 });
	ImageDrawPixel(&icon,16,36,CLITERAL(Color){ 52, 23, 11, 255 });
	ImageDrawPixel(&icon,16,37,CLITERAL(Color){ 77, 31, 13, 255 });
	ImageDrawPixel(&icon,16,38,CLITERAL(Color){ 71, 29, 12, 255 });
	ImageDrawPixel(&icon,16,39,CLITERAL(Color){ 75, 30, 11, 255 });
	ImageDrawPixel(&icon,16,40,CLITERAL(Color){ 72, 27, 9, 255 });
	ImageDrawPixel(&icon,16,41,CLITERAL(Color){ 82, 29, 7, 255 });
	ImageDrawPixel(&icon,16,42,CLITERAL(Color){ 113, 37, 5, 255 });
	ImageDrawPixel(&icon,16,43,CLITERAL(Color){ 148, 44, 2, 255 });
	ImageDrawPixel(&icon,16,44,CLITERAL(Color){ 185, 42, 0, 255 });
	ImageDrawPixel(&icon,16,45,CLITERAL(Color){ 150, 10, 0, 255 });
	ImageDrawPixel(&icon,16,46,CLITERAL(Color){ 83, 7, 0, 255 });
	ImageDrawPixel(&icon,16,47,CLITERAL(Color){ 117, 109, 106, 164 });
	ImageDrawPixel(&icon,16,48,CLITERAL(Color){ 172, 195, 209, 59 });
	ImageDrawPixel(&icon,16,49,CLITERAL(Color){ 98, 117, 83, 187 });
	ImageDrawPixel(&icon,16,50,CLITERAL(Color){ 38, 71, 0, 255 });
	ImageDrawPixel(&icon,16,51,CLITERAL(Color){ 95, 131, 0, 255 });
	ImageDrawPixel(&icon,16,52,CLITERAL(Color){ 90, 121, 0, 255 });
	ImageDrawPixel(&icon,16,53,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,16,54,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,16,55,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,16,56,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,16,57,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,16,58,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,16,59,CLITERAL(Color){ 82, 121, 0, 255 });
	ImageDrawPixel(&icon,16,60,CLITERAL(Color){ 48, 70, 0, 255 });
	ImageDrawPixel(&icon,16,61,CLITERAL(Color){ 222, 218, 234, 26 });
	ImageDrawPixel(&icon,17,16,CLITERAL(Color){ 187, 183, 205, 58 });
	ImageDrawPixel(&icon,17,17,CLITERAL(Color){ 111, 115, 109, 155 });
	ImageDrawPixel(&icon,17,18,CLITERAL(Color){ 65, 82, 31, 232 });
	ImageDrawPixel(&icon,17,19,CLITERAL(Color){ 41, 70, 0, 255 });
	ImageDrawPixel(&icon,17,20,CLITERAL(Color){ 67, 101, 0, 255 });
	ImageDrawPixel(&icon,17,21,CLITERAL(Color){ 98, 146, 0, 255 });
	ImageDrawPixel(&icon,17,22,CLITERAL(Color){ 52, 106, 6, 255 });
	ImageDrawPixel(&icon,17,23,CLITERAL(Color){ 65, 41, 9, 255 });
	ImageDrawPixel(&icon,17,24,CLITERAL(Color){ 150, 24, 5, 255 });
	ImageDrawPixel(&icon,17,25,CLITERAL(Color){ 205, 47, 0, 255 });
	ImageDrawPixel(&icon,17,26,CLITERAL(Color){ 153, 47, 3, 255 });
	ImageDrawPixel(&icon,17,27,CLITERAL(Color){ 94, 33, 7, 255 });
	ImageDrawPixel(&icon,17,28,CLITERAL(Color){ 67, 27, 10, 255 });
	ImageDrawPixel(&icon,17,29,CLITERAL(Color){ 68, 29, 13, 255 });
	ImageDrawPixel(&icon,17,30,CLITERAL(Color){ 76, 31, 13, 255 });
	ImageDrawPixel(&icon,17,31,CLITERAL(Color){ 93, 36, 13, 255 });
	ImageDrawPixel(&icon,17,32,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,17,33,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,17,34,CLITERAL(Color){ 110, 41, 14, 255 });
	ImageDrawPixel(&icon,17,35,CLITERAL(Color){ 79, 30, 10, 255 });
	ImageDrawPixel(&icon,17,36,CLITERAL(Color){ 81, 30, 10, 255 });
	ImageDrawPixel(&icon,17,37,CLITERAL(Color){ 109, 40, 14, 255 });
	ImageDrawPixel(&icon,17,38,CLITERAL(Color){ 102, 38, 13, 255 });
	ImageDrawPixel(&icon,17,39,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,17,40,CLITERAL(Color){ 98, 37, 13, 255 });
	ImageDrawPixel(&icon,17,41,CLITERAL(Color){ 83, 33, 13, 255 });
	ImageDrawPixel(&icon,17,42,CLITERAL(Color){ 70, 29, 13, 255 });
	ImageDrawPixel(&icon,17,43,CLITERAL(Color){ 65, 27, 11, 255 });
	ImageDrawPixel(&icon,17,44,CLITERAL(Color){ 82, 31, 9, 255 });
	ImageDrawPixel(&icon,17,45,CLITERAL(Color){ 132, 43, 5, 255 });
	ImageDrawPixel(&icon,17,46,CLITERAL(Color){ 190, 46, 0, 255 });
	ImageDrawPixel(&icon,17,47,CLITERAL(Color){ 156, 17, 0, 255 });
	ImageDrawPixel(&icon,17,48,CLITERAL(Color){ 62, 6, 0, 255 });
	ImageDrawPixel(&icon,17,49,CLITERAL(Color){ 21, 64, 0, 255 });
	ImageDrawPixel(&icon,17,50,CLITERAL(Color){ 94, 151, 0, 255 });
	ImageDrawPixel(&icon,17,51,CLITERAL(Color){ 96, 125, 0, 255 });
	ImageDrawPixel(&icon,17,52,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,17,53,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,17,54,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,17,55,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,17,56,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,17,57,CLITERAL(Color){ 99, 132, 0, 255 });
	ImageDrawPixel(&icon,17,58,CLITERAL(Color){ 80, 120, 0, 255 });
	ImageDrawPixel(&icon,17,59,CLITERAL(Color){ 14, 43, 0, 255 });
	ImageDrawPixel(&icon,17,60,CLITERAL(Color){ 139, 140, 141, 126 });
	ImageDrawPixel(&icon,18,18,CLITERAL(Color){ 231, 224, 250, 9 });
	ImageDrawPixel(&icon,18,19,CLITERAL(Color){ 168, 164, 178, 89 });
	ImageDrawPixel(&icon,18,20,CLITERAL(Color){ 95, 123, 92, 168 });
	ImageDrawPixel(&icon,18,21,CLITERAL(Color){ 32, 73, 4, 255 });
	ImageDrawPixel(&icon,18,22,CLITERAL(Color){ 94, 29, 8, 255 });
	ImageDrawPixel(&icon,18,23,CLITERAL(Color){ 197, 39, 2, 255 });
	ImageDrawPixel(&icon,18,24,CLITERAL(Color){ 182, 52, 0, 255 });
	ImageDrawPixel(&icon,18,25,CLITERAL(Color){ 95, 35, 7, 255 });
	ImageDrawPixel(&icon,18,26,CLITERAL(Color){ 63, 28, 12, 255 });
	ImageDrawPixel(&icon,18,27,CLITERAL(Color){ 72, 30, 13, 255 });
	ImageDrawPixel(&icon,18,28,CLITERAL(Color){ 97, 37, 13, 255 });
	ImageDrawPixel(&icon,18,29,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,18,30,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,18,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,18,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,18,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,18,34,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,18,35,CLITERAL(Color){ 75, 29, 10, 255 });
	ImageDrawPixel(&icon,18,36,CLITERAL(Color){ 77, 29, 10, 255 });
	ImageDrawPixel(&icon,18,37,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,18,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,18,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,18,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,18,41,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,18,42,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,18,43,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,18,44,CLITERAL(Color){ 82, 33, 14, 255 });
	ImageDrawPixel(&icon,18,45,CLITERAL(Color){ 61, 27, 12, 255 });
	ImageDrawPixel(&icon,18,46,CLITERAL(Color){ 77, 31, 9, 255 });
	ImageDrawPixel(&icon,18,47,CLITERAL(Color){ 151, 45, 2, 255 });
	ImageDrawPixel(&icon,18,48,CLITERAL(Color){ 205, 47, 0, 255 });
	ImageDrawPixel(&icon,18,49,CLITERAL(Color){ 126, 28, 5, 255 });
	ImageDrawPixel(&icon,18,50,CLITERAL(Color){ 41, 66, 9, 255 });
	ImageDrawPixel(&icon,18,51,CLITERAL(Color){ 75, 132, 0, 255 });
	ImageDrawPixel(&icon,18,52,CLITERAL(Color){ 98, 128, 0, 255 });
	ImageDrawPixel(&icon,18,53,CLITERAL(Color){ 86, 116, 0, 255 });
	ImageDrawPixel(&icon,18,54,CLITERAL(Color){ 93, 125, 0, 255 });
	ImageDrawPixel(&icon,18,55,CLITERAL(Color){ 97, 133, 0, 255 });
	ImageDrawPixel(&icon,18,56,CLITERAL(Color){ 71, 109, 0, 255 });
	ImageDrawPixel(&icon,18,57,CLITERAL(Color){ 34, 67, 0, 255 });
	ImageDrawPixel(&icon,18,58,CLITERAL(Color){ 42, 62, 17, 249 });
	ImageDrawPixel(&icon,18,59,CLITERAL(Color){ 145, 144, 151, 120 });
	ImageDrawPixel(&icon,19,20,CLITERAL(Color){ 128, 138, 156, 105 });
	ImageDrawPixel(&icon,19,21,CLITERAL(Color){ 86, 0, 3, 255 });
	ImageDrawPixel(&icon,19,22,CLITERAL(Color){ 208, 48, 0, 255 });
	ImageDrawPixel(&icon,19,23,CLITERAL(Color){ 148, 47, 4, 255 });
	ImageDrawPixel(&icon,19,24,CLITERAL(Color){ 67, 28, 11, 255 });
	ImageDrawPixel(&icon,19,25,CLITERAL(Color){ 66, 29, 14, 255 });
	ImageDrawPixel(&icon,19,26,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,19,27,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,19,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,34,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,19,35,CLITERAL(Color){ 76, 29, 10, 255 });
	ImageDrawPixel(&icon,19,36,CLITERAL(Color){ 77, 29, 10, 255 });
	ImageDrawPixel(&icon,19,37,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,19,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,19,43,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,19,44,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,19,45,CLITERAL(Color){ 105, 38, 12, 255 });
	ImageDrawPixel(&icon,19,46,CLITERAL(Color){ 79, 33, 14, 255 });
	ImageDrawPixel(&icon,19,47,CLITERAL(Color){ 59, 27, 12, 255 });
	ImageDrawPixel(&icon,19,48,CLITERAL(Color){ 111, 39, 6, 255 });
	ImageDrawPixel(&icon,19,49,CLITERAL(Color){ 203, 51, 0, 255 });
	ImageDrawPixel(&icon,19,50,CLITERAL(Color){ 147, 22, 5, 255 });
	ImageDrawPixel(&icon,19,51,CLITERAL(Color){ 41, 55, 8, 255 });
	ImageDrawPixel(&icon,19,52,CLITERAL(Color){ 80, 138, 2, 255 });
	ImageDrawPixel(&icon,19,53,CLITERAL(Color){ 97, 133, 0, 255 });
	ImageDrawPixel(&icon,19,54,CLITERAL(Color){ 46, 81, 0, 255 });
	ImageDrawPixel(&icon,19,55,CLITERAL(Color){ 38, 63, 0, 255 });
	ImageDrawPixel(&icon,19,56,CLITERAL(Color){ 79, 94, 51, 215 });
	ImageDrawPixel(&icon,19,57,CLITERAL(Color){ 126, 128, 133, 133 });
	ImageDrawPixel(&icon,19,58,CLITERAL(Color){ 218, 212, 234, 30 });
	ImageDrawPixel(&icon,20,19,CLITERAL(Color){ 132, 148, 146, 112 });
	ImageDrawPixel(&icon,20,20,CLITERAL(Color){ 84, 0, 0, 255 });
	ImageDrawPixel(&icon,20,21,CLITERAL(Color){ 207, 50, 0, 255 });
	ImageDrawPixel(&icon,20,22,CLITERAL(Color){ 127, 42, 5, 255 });
	ImageDrawPixel(&icon,20,23,CLITERAL(Color){ 57, 27, 13, 255 });
	ImageDrawPixel(&icon,20,24,CLITERAL(Color){ 86, 34, 14, 255 });
	ImageDrawPixel(&icon,20,25,CLITERAL(Color){ 105, 38, 12, 255 });
	ImageDrawPixel(&icon,20,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,34,CLITERAL(Color){ 103, 39, 13, 255 });
	ImageDrawPixel(&icon,20,35,CLITERAL(Color){ 77, 29, 10, 255 });
	ImageDrawPixel(&icon,20,36,CLITERAL(Color){ 79, 29, 10, 255 });
	ImageDrawPixel(&icon,20,37,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,20,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,20,45,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,20,46,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,20,47,CLITERAL(Color){ 99, 37, 13, 255 });
	ImageDrawPixel(&icon,20,48,CLITERAL(Color){ 60, 28, 14, 255 });
	ImageDrawPixel(&icon,20,49,CLITERAL(Color){ 87, 33, 8, 255 });
	ImageDrawPixel(&icon,20,50,CLITERAL(Color){ 198, 53, 0, 255 });
	ImageDrawPixel(&icon,20,51,CLITERAL(Color){ 157, 27, 5, 255 });
	ImageDrawPixel(&icon,20,52,CLITERAL(Color){ 34, 53, 4, 255 });
	ImageDrawPixel(&icon,20,53,CLITERAL(Color){ 72, 113, 39, 223 });
	ImageDrawPixel(&icon,20,54,CLITERAL(Color){ 110, 116, 103, 163 });
	ImageDrawPixel(&icon,20,55,CLITERAL(Color){ 172, 168, 189, 75 });
	ImageDrawPixel(&icon,21,18,CLITERAL(Color){ 170, 192, 196, 66 });
	ImageDrawPixel(&icon,21,19,CLITERAL(Color){ 66, 0, 0, 255 });
	ImageDrawPixel(&icon,21,20,CLITERAL(Color){ 201, 42, 0, 255 });
	ImageDrawPixel(&icon,21,21,CLITERAL(Color){ 128, 43, 5, 255 });
	ImageDrawPixel(&icon,21,22,CLITERAL(Color){ 54, 26, 13, 255 });
	ImageDrawPixel(&icon,21,23,CLITERAL(Color){ 94, 36, 13, 255 });
	ImageDrawPixel(&icon,21,24,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,21,25,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,21,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,32,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,21,33,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,21,34,CLITERAL(Color){ 114, 42, 13, 255 });
	ImageDrawPixel(&icon,21,35,CLITERAL(Color){ 67, 27, 10, 255 });
	ImageDrawPixel(&icon,21,36,CLITERAL(Color){ 70, 27, 10, 255 });
	ImageDrawPixel(&icon,21,37,CLITERAL(Color){ 113, 41, 13, 255 });
	ImageDrawPixel(&icon,21,38,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,21,39,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,21,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,46,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,21,47,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,21,48,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,21,49,CLITERAL(Color){ 66, 29, 14, 255 });
	ImageDrawPixel(&icon,21,50,CLITERAL(Color){ 84, 33, 9, 255 });
	ImageDrawPixel(&icon,21,51,CLITERAL(Color){ 204, 55, 0, 255 });
	ImageDrawPixel(&icon,21,52,CLITERAL(Color){ 104, 0, 0, 255 });
	ImageDrawPixel(&icon,21,53,CLITERAL(Color){ 103, 111, 130, 147 });
	ImageDrawPixel(&icon,22,17,CLITERAL(Color){ 218, 238, 245, 6 });
	ImageDrawPixel(&icon,22,18,CLITERAL(Color){ 69, 31, 19, 246 });
	ImageDrawPixel(&icon,22,19,CLITERAL(Color){ 180, 25, 0, 255 });
	ImageDrawPixel(&icon,22,20,CLITERAL(Color){ 136, 45, 4, 255 });
	ImageDrawPixel(&icon,22,21,CLITERAL(Color){ 51, 25, 13, 255 });
	ImageDrawPixel(&icon,22,22,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,22,23,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,22,24,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,22,25,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,32,CLITERAL(Color){ 106, 39, 13, 255 });
	ImageDrawPixel(&icon,22,33,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,22,34,CLITERAL(Color){ 69, 27, 10, 255 });
	ImageDrawPixel(&icon,22,35,CLITERAL(Color){ 63, 25, 10, 255 });
	ImageDrawPixel(&icon,22,36,CLITERAL(Color){ 63, 26, 11, 255 });
	ImageDrawPixel(&icon,22,37,CLITERAL(Color){ 70, 28, 11, 255 });
	ImageDrawPixel(&icon,22,38,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,22,39,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,22,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,46,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,22,48,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,22,49,CLITERAL(Color){ 112, 41, 13, 255 });
	ImageDrawPixel(&icon,22,50,CLITERAL(Color){ 64, 29, 14, 255 });
	ImageDrawPixel(&icon,22,51,CLITERAL(Color){ 85, 31, 7, 255 });
	ImageDrawPixel(&icon,22,52,CLITERAL(Color){ 202, 46, 0, 255 });
	ImageDrawPixel(&icon,22,53,CLITERAL(Color){ 89, 16, 7, 252 });
	ImageDrawPixel(&icon,22,54,CLITERAL(Color){ 187, 207, 212, 52 });
	ImageDrawPixel(&icon,23,6,CLITERAL(Color){ 236, 230, 253, 3 });
	ImageDrawPixel(&icon,23,7,CLITERAL(Color){ 171, 168, 183, 76 });
	ImageDrawPixel(&icon,23,8,CLITERAL(Color){ 140, 143, 135, 126 });
	ImageDrawPixel(&icon,23,9,CLITERAL(Color){ 123, 132, 104, 158 });
	ImageDrawPixel(&icon,23,10,CLITERAL(Color){ 107, 118, 80, 171 });
	ImageDrawPixel(&icon,23,11,CLITERAL(Color){ 105, 115, 77, 171 });
	ImageDrawPixel(&icon,23,12,CLITERAL(Color){ 117, 124, 102, 151 });
	ImageDrawPixel(&icon,23,13,CLITERAL(Color){ 149, 148, 156, 101 });
	ImageDrawPixel(&icon,23,14,CLITERAL(Color){ 219, 213, 236, 20 });
	ImageDrawPixel(&icon,23,17,CLITERAL(Color){ 103, 103, 100, 161 });
	ImageDrawPixel(&icon,23,18,CLITERAL(Color){ 124, 0, 0, 255 });
	ImageDrawPixel(&icon,23,19,CLITERAL(Color){ 179, 55, 2, 255 });
	ImageDrawPixel(&icon,23,20,CLITERAL(Color){ 57, 26, 11, 255 });
	ImageDrawPixel(&icon,23,21,CLITERAL(Color){ 56, 25, 12, 255 });
	ImageDrawPixel(&icon,23,22,CLITERAL(Color){ 79, 31, 11, 255 });
	ImageDrawPixel(&icon,23,23,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,23,24,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,23,25,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,23,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,23,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,23,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,23,29,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,23,30,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,23,31,CLITERAL(Color){ 104, 38, 13, 255 });
	ImageDrawPixel(&icon,23,32,CLITERAL(Color){ 73, 28, 10, 255 });
	ImageDrawPixel(&icon,23,33,CLITERAL(Color){ 60, 24, 10, 255 });
	ImageDrawPixel(&icon,23,34,CLITERAL(Color){ 70, 28, 11, 255 });
	ImageDrawPixel(&icon,23,35,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,23,36,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,23,37,CLITERAL(Color){ 70, 28, 11, 255 });
	ImageDrawPixel(&icon,23,38,CLITERAL(Color){ 61, 25, 11, 255 });
	ImageDrawPixel(&icon,23,39,CLITERAL(Color){ 76, 30, 11, 255 });
	ImageDrawPixel(&icon,23,40,CLITERAL(Color){ 104, 39, 12, 255 });
	ImageDrawPixel(&icon,23,41,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,23,42,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,23,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,23,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,23,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,23,46,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,23,47,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,23,48,CLITERAL(Color){ 105, 39, 13, 255 });
	ImageDrawPixel(&icon,23,49,CLITERAL(Color){ 80, 31, 11, 255 });
	ImageDrawPixel(&icon,23,50,CLITERAL(Color){ 68, 27, 11, 255 });
	ImageDrawPixel(&icon,23,51,CLITERAL(Color){ 45, 23, 13, 255 });
	ImageDrawPixel(&icon,23,52,CLITERAL(Color){ 133, 45, 5, 255 });
	ImageDrawPixel(&icon,23,53,CLITERAL(Color){ 168, 15, 0, 255 });
	ImageDrawPixel(&icon,23,54,CLITERAL(Color){ 87, 60, 44, 225 });
	ImageDrawPixel(&icon,24,4,CLITERAL(Color){ 228, 222, 241, 18 });
	ImageDrawPixel(&icon,24,5,CLITERAL(Color){ 121, 123, 129, 136 });
	ImageDrawPixel(&icon,24,6,CLITERAL(Color){ 65, 81, 36, 226 });
	ImageDrawPixel(&icon,24,7,CLITERAL(Color){ 43, 68, 0, 255 });
	ImageDrawPixel(&icon,24,8,CLITERAL(Color){ 42, 72, 0, 255 });
	ImageDrawPixel(&icon,24,9,CLITERAL(Color){ 52, 86, 0, 255 });
	ImageDrawPixel(&icon,24,10,CLITERAL(Color){ 62, 98, 0, 255 });
	ImageDrawPixel(&icon,24,11,CLITERAL(Color){ 65, 100, 0, 255 });
	ImageDrawPixel(&icon,24,12,CLITERAL(Color){ 51, 85, 0, 255 });
	ImageDrawPixel(&icon,24,13,CLITERAL(Color){ 38, 66, 0, 255 });
	ImageDrawPixel(&icon,24,14,CLITERAL(Color){ 52, 70, 21, 242 });
	ImageDrawPixel(&icon,24,15,CLITERAL(Color){ 147, 148, 148, 118 });
	ImageDrawPixel(&icon,24,16,CLITERAL(Color){ 175, 194, 211, 49 });
	ImageDrawPixel(&icon,24,17,CLITERAL(Color){ 87, 22, 0, 255 });
	ImageDrawPixel(&icon,24,18,CLITERAL(Color){ 200, 46, 0, 255 });
	ImageDrawPixel(&icon,24,19,CLITERAL(Color){ 92, 33, 7, 255 });
	ImageDrawPixel(&icon,24,20,CLITERAL(Color){ 73, 31, 14, 255 });
	ImageDrawPixel(&icon,24,21,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,24,22,CLITERAL(Color){ 67, 27, 11, 255 });
	ImageDrawPixel(&icon,24,23,CLITERAL(Color){ 65, 27, 11, 255 });
	ImageDrawPixel(&icon,24,24,CLITERAL(Color){ 86, 33, 12, 255 });
	ImageDrawPixel(&icon,24,25,CLITERAL(Color){ 105, 39, 12, 255 });
	ImageDrawPixel(&icon,24,26,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,24,27,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,24,28,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,24,29,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,24,30,CLITERAL(Color){ 85, 33, 12, 255 });
	ImageDrawPixel(&icon,24,31,CLITERAL(Color){ 62, 25, 10, 255 });
	ImageDrawPixel(&icon,24,32,CLITERAL(Color){ 62, 25, 10, 255 });
	ImageDrawPixel(&icon,24,33,CLITERAL(Color){ 91, 34, 11, 255 });
	ImageDrawPixel(&icon,24,34,CLITERAL(Color){ 108, 40, 13, 255 });
	ImageDrawPixel(&icon,24,35,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,24,36,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,24,37,CLITERAL(Color){ 108, 40, 12, 255 });
	ImageDrawPixel(&icon,24,38,CLITERAL(Color){ 90, 34, 12, 255 });
	ImageDrawPixel(&icon,24,39,CLITERAL(Color){ 63, 26, 11, 255 });
	ImageDrawPixel(&icon,24,40,CLITERAL(Color){ 61, 25, 10, 255 });
	ImageDrawPixel(&icon,24,41,CLITERAL(Color){ 87, 34, 12, 255 });
	ImageDrawPixel(&icon,24,42,CLITERAL(Color){ 107, 39, 12, 255 });
	ImageDrawPixel(&icon,24,43,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,24,44,CLITERAL(Color){ 94, 36, 12, 255 });
	ImageDrawPixel(&icon,24,45,CLITERAL(Color){ 98, 36, 12, 255 });
	ImageDrawPixel(&icon,24,46,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,24,47,CLITERAL(Color){ 90, 34, 12, 255 });
	ImageDrawPixel(&icon,24,48,CLITERAL(Color){ 66, 27, 11, 255 });
	ImageDrawPixel(&icon,24,49,CLITERAL(Color){ 65, 26, 11, 255 });
	ImageDrawPixel(&icon,24,50,CLITERAL(Color){ 92, 34, 11, 255 });
	ImageDrawPixel(&icon,24,51,CLITERAL(Color){ 97, 37, 14, 255 });
	ImageDrawPixel(&icon,24,52,CLITERAL(Color){ 66, 28, 10, 255 });
	ImageDrawPixel(&icon,24,53,CLITERAL(Color){ 181, 52, 0, 255 });
	ImageDrawPixel(&icon,24,54,CLITERAL(Color){ 94, 0, 0, 255 });
	ImageDrawPixel(&icon,24,55,CLITERAL(Color){ 153, 167, 171, 94 });
	ImageDrawPixel(&icon,25,3,CLITERAL(Color){ 162, 158, 171, 96 });
	ImageDrawPixel(&icon,25,4,CLITERAL(Color){ 48, 65, 24, 239 });
	ImageDrawPixel(&icon,25,5,CLITERAL(Color){ 35, 67, 0, 255 });
	ImageDrawPixel(&icon,25,6,CLITERAL(Color){ 80, 119, 0, 255 });
	ImageDrawPixel(&icon,25,7,CLITERAL(Color){ 113, 154, 0, 255 });
	ImageDrawPixel(&icon,25,8,CLITERAL(Color){ 106, 144, 0, 255 });
	ImageDrawPixel(&icon,25,9,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,25,10,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,25,11,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,25,12,CLITERAL(Color){ 92, 123, 0, 255 });
	ImageDrawPixel(&icon,25,13,CLITERAL(Color){ 98, 133, 0, 255 });
	ImageDrawPixel(&icon,25,14,CLITERAL(Color){ 81, 119, 0, 255 });
	ImageDrawPixel(&icon,25,15,CLITERAL(Color){ 45, 90, 0, 255 });
	ImageDrawPixel(&icon,25,16,CLITERAL(Color){ 35, 35, 3, 255 });
	ImageDrawPixel(&icon,25,17,CLITERAL(Color){ 167, 33, 0, 255 });
	ImageDrawPixel(&icon,25,18,CLITERAL(Color){ 150, 47, 2, 255 });
	ImageDrawPixel(&icon,25,19,CLITERAL(Color){ 63, 28, 11, 255 });
	ImageDrawPixel(&icon,25,20,CLITERAL(Color){ 101, 37, 13, 255 });
	ImageDrawPixel(&icon,25,21,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,25,22,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,25,23,CLITERAL(Color){ 87, 34, 12, 255 });
	ImageDrawPixel(&icon,25,24,CLITERAL(Color){ 66, 28, 11, 255 });
	ImageDrawPixel(&icon,25,25,CLITERAL(Color){ 67, 27, 11, 255 });
	ImageDrawPixel(&icon,25,26,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,25,27,CLITERAL(Color){ 112, 41, 13, 255 });
	ImageDrawPixel(&icon,25,28,CLITERAL(Color){ 92, 35, 12, 255 });
	ImageDrawPixel(&icon,25,29,CLITERAL(Color){ 64, 26, 11, 255 });
	ImageDrawPixel(&icon,25,30,CLITERAL(Color){ 61, 25, 11, 255 });
	ImageDrawPixel(&icon,25,31,CLITERAL(Color){ 84, 32, 11, 255 });
	ImageDrawPixel(&icon,25,32,CLITERAL(Color){ 107, 39, 13, 255 });
	ImageDrawPixel(&icon,25,33,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,25,34,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,25,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,25,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,25,37,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,25,38,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,25,39,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,25,40,CLITERAL(Color){ 82, 32, 11, 255 });
	ImageDrawPixel(&icon,25,41,CLITERAL(Color){ 62, 25, 10, 255 });
	ImageDrawPixel(&icon,25,42,CLITERAL(Color){ 65, 26, 10, 255 });
	ImageDrawPixel(&icon,25,43,CLITERAL(Color){ 94, 36, 12, 255 });
	ImageDrawPixel(&icon,25,44,CLITERAL(Color){ 112, 41, 13, 255 });
	ImageDrawPixel(&icon,25,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,25,46,CLITERAL(Color){ 71, 28, 11, 255 });
	ImageDrawPixel(&icon,25,47,CLITERAL(Color){ 65, 27, 11, 255 });
	ImageDrawPixel(&icon,25,48,CLITERAL(Color){ 81, 31, 11, 255 });
	ImageDrawPixel(&icon,25,49,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,25,50,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,25,51,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,25,52,CLITERAL(Color){ 64, 28, 13, 255 });
	ImageDrawPixel(&icon,25,53,CLITERAL(Color){ 104, 37, 7, 255 });
	ImageDrawPixel(&icon,25,54,CLITERAL(Color){ 173, 21, 0, 255 });
	ImageDrawPixel(&icon,25,55,CLITERAL(Color){ 100, 66, 47, 221 });
	ImageDrawPixel(&icon,26,2,CLITERAL(Color){ 135, 133, 138, 129 });
	ImageDrawPixel(&icon,26,3,CLITERAL(Color){ 22, 47, 0, 255 });
	ImageDrawPixel(&icon,26,4,CLITERAL(Color){ 74, 113, 0, 255 });
	ImageDrawPixel(&icon,26,5,CLITERAL(Color){ 104, 140, 0, 255 });
	ImageDrawPixel(&icon,26,6,CLITERAL(Color){ 88, 119, 2, 255 });
	ImageDrawPixel(&icon,26,7,CLITERAL(Color){ 46, 63, 4, 255 });
	ImageDrawPixel(&icon,26,8,CLITERAL(Color){ 62, 84, 2, 255 });
	ImageDrawPixel(&icon,26,9,CLITERAL(Color){ 97, 132, 0, 255 });
	ImageDrawPixel(&icon,26,10,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,26,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,26,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,26,13,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,26,14,CLITERAL(Color){ 93, 125, 0, 255 });
	ImageDrawPixel(&icon,26,15,CLITERAL(Color){ 65, 123, 0, 255 });
	ImageDrawPixel(&icon,26,16,CLITERAL(Color){ 83, 40, 0, 255 });
	ImageDrawPixel(&icon,26,17,CLITERAL(Color){ 199, 44, 1, 255 });
	ImageDrawPixel(&icon,26,18,CLITERAL(Color){ 94, 34, 7, 255 });
	ImageDrawPixel(&icon,26,19,CLITERAL(Color){ 70, 30, 14, 255 });
	ImageDrawPixel(&icon,26,20,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,26,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,22,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,26,23,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,26,24,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,26,25,CLITERAL(Color){ 79, 32, 12, 255 });
	ImageDrawPixel(&icon,26,26,CLITERAL(Color){ 67, 27, 11, 255 });
	ImageDrawPixel(&icon,26,27,CLITERAL(Color){ 67, 27, 10, 255 });
	ImageDrawPixel(&icon,26,28,CLITERAL(Color){ 62, 26, 10, 255 });
	ImageDrawPixel(&icon,26,29,CLITERAL(Color){ 77, 30, 11, 255 });
	ImageDrawPixel(&icon,26,30,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,26,31,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,26,32,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,26,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,39,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,26,40,CLITERAL(Color){ 104, 39, 12, 255 });
	ImageDrawPixel(&icon,26,41,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,26,42,CLITERAL(Color){ 75, 30, 11, 255 });
	ImageDrawPixel(&icon,26,43,CLITERAL(Color){ 63, 26, 11, 255 });
	ImageDrawPixel(&icon,26,44,CLITERAL(Color){ 71, 28, 11, 255 });
	ImageDrawPixel(&icon,26,45,CLITERAL(Color){ 68, 28, 11, 255 });
	ImageDrawPixel(&icon,26,46,CLITERAL(Color){ 75, 30, 11, 255 });
	ImageDrawPixel(&icon,26,47,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,26,48,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,26,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,26,52,CLITERAL(Color){ 95, 37, 14, 255 });
	ImageDrawPixel(&icon,26,53,CLITERAL(Color){ 70, 29, 10, 255 });
	ImageDrawPixel(&icon,26,54,CLITERAL(Color){ 174, 47, 0, 255 });
	ImageDrawPixel(&icon,26,55,CLITERAL(Color){ 95, 11, 0, 255 });
	ImageDrawPixel(&icon,26,56,CLITERAL(Color){ 159, 179, 186, 59 });
	ImageDrawPixel(&icon,27,1,CLITERAL(Color){ 159, 157, 165, 95 });
	ImageDrawPixel(&icon,27,2,CLITERAL(Color){ 17, 43, 0, 255 });
	ImageDrawPixel(&icon,27,3,CLITERAL(Color){ 89, 128, 0, 255 });
	ImageDrawPixel(&icon,27,4,CLITERAL(Color){ 95, 126, 0, 255 });
	ImageDrawPixel(&icon,27,5,CLITERAL(Color){ 98, 134, 1, 255 });
	ImageDrawPixel(&icon,27,6,CLITERAL(Color){ 28, 38, 4, 255 });
	ImageDrawPixel(&icon,27,7,CLITERAL(Color){ 0, 0, 1, 255 });
	ImageDrawPixel(&icon,27,8,CLITERAL(Color){ 0, 0, 3, 255 });
	ImageDrawPixel(&icon,27,9,CLITERAL(Color){ 71, 96, 3, 255 });
	ImageDrawPixel(&icon,27,10,CLITERAL(Color){ 100, 135, 0, 255 });
	ImageDrawPixel(&icon,27,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,27,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,27,13,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,27,14,CLITERAL(Color){ 96, 135, 0, 255 });
	ImageDrawPixel(&icon,27,15,CLITERAL(Color){ 56, 91, 3, 255 });
	ImageDrawPixel(&icon,27,16,CLITERAL(Color){ 129, 30, 5, 255 });
	ImageDrawPixel(&icon,27,17,CLITERAL(Color){ 170, 48, 2, 255 });
	ImageDrawPixel(&icon,27,18,CLITERAL(Color){ 69, 28, 10, 255 });
	ImageDrawPixel(&icon,27,19,CLITERAL(Color){ 95, 36, 13, 255 });
	ImageDrawPixel(&icon,27,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,25,CLITERAL(Color){ 107, 40, 12, 255 });
	ImageDrawPixel(&icon,27,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,27,27,CLITERAL(Color){ 41, 18, 9, 255 });
	ImageDrawPixel(&icon,27,28,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,27,29,CLITERAL(Color){ 107, 39, 12, 255 });
	ImageDrawPixel(&icon,27,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,31,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,27,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,40,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,27,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,42,CLITERAL(Color){ 107, 39, 12, 255 });
	ImageDrawPixel(&icon,27,43,CLITERAL(Color){ 91, 34, 12, 255 });
	ImageDrawPixel(&icon,27,44,CLITERAL(Color){ 46, 20, 9, 255 });
	ImageDrawPixel(&icon,27,45,CLITERAL(Color){ 89, 34, 12, 255 });
	ImageDrawPixel(&icon,27,46,CLITERAL(Color){ 107, 39, 12, 255 });
	ImageDrawPixel(&icon,27,47,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,27,48,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,27,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,27,52,CLITERAL(Color){ 106, 39, 13, 255 });
	ImageDrawPixel(&icon,27,53,CLITERAL(Color){ 62, 27, 12, 255 });
	ImageDrawPixel(&icon,27,54,CLITERAL(Color){ 125, 40, 3, 255 });
	ImageDrawPixel(&icon,27,55,CLITERAL(Color){ 135, 7, 0, 255 });
	ImageDrawPixel(&icon,27,56,CLITERAL(Color){ 123, 115, 110, 144 });
	ImageDrawPixel(&icon,28,0,CLITERAL(Color){ 226, 220, 239, 20 });
	ImageDrawPixel(&icon,28,1,CLITERAL(Color){ 47, 64, 7, 255 });
	ImageDrawPixel(&icon,28,2,CLITERAL(Color){ 78, 117, 0, 255 });
	ImageDrawPixel(&icon,28,3,CLITERAL(Color){ 92, 123, 0, 255 });
	ImageDrawPixel(&icon,28,4,CLITERAL(Color){ 90, 122, 0, 255 });
	ImageDrawPixel(&icon,28,5,CLITERAL(Color){ 99, 134, 1, 255 });
	ImageDrawPixel(&icon,28,6,CLITERAL(Color){ 19, 25, 4, 255 });
	ImageDrawPixel(&icon,28,7,CLITERAL(Color){ 0, 0, 1, 255 });
	ImageDrawPixel(&icon,28,8,CLITERAL(Color){ 0, 0, 3, 255 });
	ImageDrawPixel(&icon,28,9,CLITERAL(Color){ 64, 87, 3, 255 });
	ImageDrawPixel(&icon,28,10,CLITERAL(Color){ 102, 138, 0, 255 });
	ImageDrawPixel(&icon,28,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,28,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,28,13,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,28,14,CLITERAL(Color){ 89, 136, 0, 255 });
	ImageDrawPixel(&icon,28,15,CLITERAL(Color){ 60, 65, 4, 255 });
	ImageDrawPixel(&icon,28,16,CLITERAL(Color){ 161, 29, 4, 255 });
	ImageDrawPixel(&icon,28,17,CLITERAL(Color){ 134, 41, 3, 255 });
	ImageDrawPixel(&icon,28,18,CLITERAL(Color){ 69, 29, 12, 255 });
	ImageDrawPixel(&icon,28,19,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,28,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,25,CLITERAL(Color){ 97, 37, 12, 255 });
	ImageDrawPixel(&icon,28,26,CLITERAL(Color){ 91, 34, 11, 255 });
	ImageDrawPixel(&icon,28,27,CLITERAL(Color){ 62, 24, 8, 255 });
	ImageDrawPixel(&icon,28,28,CLITERAL(Color){ 96, 35, 12, 255 });
	ImageDrawPixel(&icon,28,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,43,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,28,44,CLITERAL(Color){ 70, 26, 9, 255 });
	ImageDrawPixel(&icon,28,45,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,28,46,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,28,52,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,28,53,CLITERAL(Color){ 75, 31, 13, 255 });
	ImageDrawPixel(&icon,28,54,CLITERAL(Color){ 101, 34, 6, 255 });
	ImageDrawPixel(&icon,28,55,CLITERAL(Color){ 160, 19, 0, 255 });
	ImageDrawPixel(&icon,28,56,CLITERAL(Color){ 110, 75, 60, 214 });
	ImageDrawPixel(&icon,28,62,CLITERAL(Color){ 247, 241, 255, 7 });
	ImageDrawPixel(&icon,28,63,CLITERAL(Color){ 147, 145, 155, 108 });
	ImageDrawPixel(&icon,29,0,CLITERAL(Color){ 134, 135, 133, 129 });
	ImageDrawPixel(&icon,29,1,CLITERAL(Color){ 31, 62, 0, 255 });
	ImageDrawPixel(&icon,29,2,CLITERAL(Color){ 100, 133, 0, 255 });
	ImageDrawPixel(&icon,29,3,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,29,4,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,29,5,CLITERAL(Color){ 95, 128, 0, 255 });
	ImageDrawPixel(&icon,29,6,CLITERAL(Color){ 81, 110, 2, 255 });
	ImageDrawPixel(&icon,29,7,CLITERAL(Color){ 35, 48, 5, 255 });
	ImageDrawPixel(&icon,29,8,CLITERAL(Color){ 52, 70, 3, 255 });
	ImageDrawPixel(&icon,29,9,CLITERAL(Color){ 95, 129, 0, 255 });
	ImageDrawPixel(&icon,29,10,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,29,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,29,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,29,13,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,29,14,CLITERAL(Color){ 76, 128, 0, 255 });
	ImageDrawPixel(&icon,29,15,CLITERAL(Color){ 70, 53, 5, 255 });
	ImageDrawPixel(&icon,29,16,CLITERAL(Color){ 180, 37, 3, 255 });
	ImageDrawPixel(&icon,29,17,CLITERAL(Color){ 113, 36, 4, 255 });
	ImageDrawPixel(&icon,29,18,CLITERAL(Color){ 71, 29, 12, 255 });
	ImageDrawPixel(&icon,29,19,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,29,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,29,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,29,27,CLITERAL(Color){ 59, 23, 8, 255 });
	ImageDrawPixel(&icon,29,28,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,29,29,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,29,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,42,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,29,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,29,44,CLITERAL(Color){ 66, 25, 9, 255 });
	ImageDrawPixel(&icon,29,45,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,29,46,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,29,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,52,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,29,53,CLITERAL(Color){ 85, 34, 13, 255 });
	ImageDrawPixel(&icon,29,54,CLITERAL(Color){ 87, 31, 7, 255 });
	ImageDrawPixel(&icon,29,55,CLITERAL(Color){ 176, 40, 0, 255 });
	ImageDrawPixel(&icon,29,56,CLITERAL(Color){ 88, 35, 14, 249 });
	ImageDrawPixel(&icon,29,57,CLITERAL(Color){ 173, 190, 214, 52 });
	ImageDrawPixel(&icon,29,58,CLITERAL(Color){ 235, 229, 252, 13 });
	ImageDrawPixel(&icon,29,59,CLITERAL(Color){ 232, 226, 249, 13 });
	ImageDrawPixel(&icon,29,60,CLITERAL(Color){ 208, 203, 225, 38 });
	ImageDrawPixel(&icon,29,61,CLITERAL(Color){ 157, 155, 166, 101 });
	ImageDrawPixel(&icon,29,62,CLITERAL(Color){ 94, 112, 52, 224 });
	ImageDrawPixel(&icon,29,63,CLITERAL(Color){ 1, 19, 0, 255 });
	ImageDrawPixel(&icon,30,0,CLITERAL(Color){ 90, 103, 56, 209 });
	ImageDrawPixel(&icon,30,1,CLITERAL(Color){ 66, 103, 0, 255 });
	ImageDrawPixel(&icon,30,2,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,30,3,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,30,4,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,30,5,CLITERAL(Color){ 86, 116, 0, 255 });
	ImageDrawPixel(&icon,30,6,CLITERAL(Color){ 94, 128, 0, 255 });
	ImageDrawPixel(&icon,30,7,CLITERAL(Color){ 102, 138, 0, 255 });
	ImageDrawPixel(&icon,30,8,CLITERAL(Color){ 100, 136, 0, 255 });
	ImageDrawPixel(&icon,30,9,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,30,10,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,30,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,30,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,30,13,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,30,14,CLITERAL(Color){ 67, 120, 0, 255 });
	ImageDrawPixel(&icon,30,15,CLITERAL(Color){ 84, 48, 4, 255 });
	ImageDrawPixel(&icon,30,16,CLITERAL(Color){ 189, 43, 3, 255 });
	ImageDrawPixel(&icon,30,17,CLITERAL(Color){ 102, 34, 6, 255 });
	ImageDrawPixel(&icon,30,18,CLITERAL(Color){ 73, 30, 13, 255 });
	ImageDrawPixel(&icon,30,19,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,30,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,30,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,30,27,CLITERAL(Color){ 59, 23, 8, 255 });
	ImageDrawPixel(&icon,30,28,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,30,29,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,30,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,42,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,30,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,30,44,CLITERAL(Color){ 66, 25, 9, 255 });
	ImageDrawPixel(&icon,30,45,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,30,46,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,30,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,52,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,30,53,CLITERAL(Color){ 95, 36, 13, 255 });
	ImageDrawPixel(&icon,30,54,CLITERAL(Color){ 77, 30, 10, 255 });
	ImageDrawPixel(&icon,30,55,CLITERAL(Color){ 174, 49, 2, 255 });
	ImageDrawPixel(&icon,30,56,CLITERAL(Color){ 103, 31, 0, 255 });
	ImageDrawPixel(&icon,30,57,CLITERAL(Color){ 27, 58, 0, 255 });
	ImageDrawPixel(&icon,30,58,CLITERAL(Color){ 75, 94, 30, 242 });
	ImageDrawPixel(&icon,30,59,CLITERAL(Color){ 75, 92, 35, 241 });
	ImageDrawPixel(&icon,30,60,CLITERAL(Color){ 51, 70, 4, 255 });
	ImageDrawPixel(&icon,30,61,CLITERAL(Color){ 49, 78, 0, 255 });
	ImageDrawPixel(&icon,30,62,CLITERAL(Color){ 46, 92, 0, 255 });
	ImageDrawPixel(&icon,30,63,CLITERAL(Color){ 73, 90, 29, 232 });
	ImageDrawPixel(&icon,31,0,CLITERAL(Color){ 51, 68, 5, 251 });
	ImageDrawPixel(&icon,31,1,CLITERAL(Color){ 101, 138, 0, 255 });
	ImageDrawPixel(&icon,31,2,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,3,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,4,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,5,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,6,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,7,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,31,8,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,9,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,10,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,31,13,CLITERAL(Color){ 91, 124, 0, 255 });
	ImageDrawPixel(&icon,31,14,CLITERAL(Color){ 62, 116, 0, 255 });
	ImageDrawPixel(&icon,31,15,CLITERAL(Color){ 96, 49, 5, 255 });
	ImageDrawPixel(&icon,31,16,CLITERAL(Color){ 192, 46, 3, 255 });
	ImageDrawPixel(&icon,31,17,CLITERAL(Color){ 96, 34, 7, 255 });
	ImageDrawPixel(&icon,31,18,CLITERAL(Color){ 75, 30, 12, 255 });
	ImageDrawPixel(&icon,31,19,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,31,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,31,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,31,27,CLITERAL(Color){ 59, 23, 8, 255 });
	ImageDrawPixel(&icon,31,28,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,31,29,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,31,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,42,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,31,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,31,44,CLITERAL(Color){ 66, 25, 9, 255 });
	ImageDrawPixel(&icon,31,45,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,31,46,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,31,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,52,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,31,53,CLITERAL(Color){ 99, 38, 14, 255 });
	ImageDrawPixel(&icon,31,54,CLITERAL(Color){ 68, 26, 9, 255 });
	ImageDrawPixel(&icon,31,55,CLITERAL(Color){ 157, 40, 2, 255 });
	ImageDrawPixel(&icon,31,56,CLITERAL(Color){ 127, 42, 6, 255 });
	ImageDrawPixel(&icon,31,57,CLITERAL(Color){ 57, 106, 0, 255 });
	ImageDrawPixel(&icon,31,58,CLITERAL(Color){ 86, 129, 0, 255 });
	ImageDrawPixel(&icon,31,59,CLITERAL(Color){ 81, 119, 0, 255 });
	ImageDrawPixel(&icon,31,60,CLITERAL(Color){ 101, 142, 0, 255 });
	ImageDrawPixel(&icon,31,61,CLITERAL(Color){ 67, 109, 0, 255 });
	ImageDrawPixel(&icon,31,62,CLITERAL(Color){ 20, 40, 0, 255 });
	ImageDrawPixel(&icon,31,63,CLITERAL(Color){ 186, 182, 197, 73 });
	ImageDrawPixel(&icon,32,0,CLITERAL(Color){ 44, 61, 0, 253 });
	ImageDrawPixel(&icon,32,1,CLITERAL(Color){ 107, 144, 0, 255 });
	ImageDrawPixel(&icon,32,2,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,3,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,4,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,5,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,6,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,7,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,8,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,9,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,10,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,32,13,CLITERAL(Color){ 91, 124, 0, 255 });
	ImageDrawPixel(&icon,32,14,CLITERAL(Color){ 62, 116, 0, 255 });
	ImageDrawPixel(&icon,32,15,CLITERAL(Color){ 96, 49, 5, 255 });
	ImageDrawPixel(&icon,32,16,CLITERAL(Color){ 192, 46, 3, 255 });
	ImageDrawPixel(&icon,32,17,CLITERAL(Color){ 96, 34, 7, 255 });
	ImageDrawPixel(&icon,32,18,CLITERAL(Color){ 75, 30, 13, 255 });
	ImageDrawPixel(&icon,32,19,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,32,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,32,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,32,27,CLITERAL(Color){ 59, 23, 8, 255 });
	ImageDrawPixel(&icon,32,28,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,32,29,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,32,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,42,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,32,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,32,44,CLITERAL(Color){ 66, 25, 9, 255 });
	ImageDrawPixel(&icon,32,45,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,32,46,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,32,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,52,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,32,53,CLITERAL(Color){ 100, 38, 13, 255 });
	ImageDrawPixel(&icon,32,54,CLITERAL(Color){ 68, 26, 9, 255 });
	ImageDrawPixel(&icon,32,55,CLITERAL(Color){ 157, 40, 1, 255 });
	ImageDrawPixel(&icon,32,56,CLITERAL(Color){ 127, 42, 7, 255 });
	ImageDrawPixel(&icon,32,57,CLITERAL(Color){ 59, 108, 0, 255 });
	ImageDrawPixel(&icon,32,58,CLITERAL(Color){ 84, 127, 0, 255 });
	ImageDrawPixel(&icon,32,59,CLITERAL(Color){ 51, 87, 0, 255 });
	ImageDrawPixel(&icon,32,60,CLITERAL(Color){ 32, 59, 0, 255 });
	ImageDrawPixel(&icon,32,61,CLITERAL(Color){ 61, 72, 54, 215 });
	ImageDrawPixel(&icon,32,62,CLITERAL(Color){ 191, 186, 198, 70 });
	ImageDrawPixel(&icon,33,0,CLITERAL(Color){ 78, 93, 39, 229 });
	ImageDrawPixel(&icon,33,1,CLITERAL(Color){ 79, 117, 0, 255 });
	ImageDrawPixel(&icon,33,2,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,33,3,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,33,4,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,33,5,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,33,6,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,33,7,CLITERAL(Color){ 101, 137, 0, 255 });
	ImageDrawPixel(&icon,33,8,CLITERAL(Color){ 97, 132, 0, 255 });
	ImageDrawPixel(&icon,33,9,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,33,10,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,33,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,33,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,33,13,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,33,14,CLITERAL(Color){ 67, 120, 0, 255 });
	ImageDrawPixel(&icon,33,15,CLITERAL(Color){ 85, 47, 5, 255 });
	ImageDrawPixel(&icon,33,16,CLITERAL(Color){ 190, 42, 2, 255 });
	ImageDrawPixel(&icon,33,17,CLITERAL(Color){ 103, 34, 6, 255 });
	ImageDrawPixel(&icon,33,18,CLITERAL(Color){ 73, 30, 13, 255 });
	ImageDrawPixel(&icon,33,19,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,33,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,33,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,33,27,CLITERAL(Color){ 59, 23, 8, 255 });
	ImageDrawPixel(&icon,33,28,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,33,29,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,33,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,42,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,33,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,33,44,CLITERAL(Color){ 66, 25, 9, 255 });
	ImageDrawPixel(&icon,33,45,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,33,46,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,33,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,52,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,33,53,CLITERAL(Color){ 94, 35, 13, 255 });
	ImageDrawPixel(&icon,33,54,CLITERAL(Color){ 78, 31, 10, 255 });
	ImageDrawPixel(&icon,33,55,CLITERAL(Color){ 176, 50, 2, 255 });
	ImageDrawPixel(&icon,33,56,CLITERAL(Color){ 104, 30, 0, 255 });
	ImageDrawPixel(&icon,33,57,CLITERAL(Color){ 27, 58, 0, 255 });
	ImageDrawPixel(&icon,33,58,CLITERAL(Color){ 77, 95, 35, 227 });
	ImageDrawPixel(&icon,33,59,CLITERAL(Color){ 117, 125, 103, 161 });
	ImageDrawPixel(&icon,33,60,CLITERAL(Color){ 162, 160, 178, 87 });
	ImageDrawPixel(&icon,34,0,CLITERAL(Color){ 110, 116, 97, 163 });
	ImageDrawPixel(&icon,34,1,CLITERAL(Color){ 40, 74, 0, 255 });
	ImageDrawPixel(&icon,34,2,CLITERAL(Color){ 96, 127, 0, 255 });
	ImageDrawPixel(&icon,34,3,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,34,4,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,34,5,CLITERAL(Color){ 91, 123, 0, 255 });
	ImageDrawPixel(&icon,34,6,CLITERAL(Color){ 92, 125, 1, 255 });
	ImageDrawPixel(&icon,34,7,CLITERAL(Color){ 65, 89, 4, 255 });
	ImageDrawPixel(&icon,34,8,CLITERAL(Color){ 76, 103, 2, 255 });
	ImageDrawPixel(&icon,34,9,CLITERAL(Color){ 96, 130, 0, 255 });
	ImageDrawPixel(&icon,34,10,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,34,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,34,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,34,13,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,34,14,CLITERAL(Color){ 75, 127, 0, 255 });
	ImageDrawPixel(&icon,34,15,CLITERAL(Color){ 70, 52, 4, 255 });
	ImageDrawPixel(&icon,34,16,CLITERAL(Color){ 181, 37, 3, 255 });
	ImageDrawPixel(&icon,34,17,CLITERAL(Color){ 115, 36, 5, 255 });
	ImageDrawPixel(&icon,34,18,CLITERAL(Color){ 71, 29, 12, 255 });
	ImageDrawPixel(&icon,34,19,CLITERAL(Color){ 102, 38, 13, 255 });
	ImageDrawPixel(&icon,34,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,34,26,CLITERAL(Color){ 90, 34, 11, 255 });
	ImageDrawPixel(&icon,34,27,CLITERAL(Color){ 59, 23, 8, 255 });
	ImageDrawPixel(&icon,34,28,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,34,29,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,34,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,42,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,34,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,34,44,CLITERAL(Color){ 66, 25, 9, 255 });
	ImageDrawPixel(&icon,34,45,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,34,46,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,34,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,34,52,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,34,53,CLITERAL(Color){ 84, 33, 13, 255 });
	ImageDrawPixel(&icon,34,54,CLITERAL(Color){ 89, 32, 8, 255 });
	ImageDrawPixel(&icon,34,55,CLITERAL(Color){ 179, 42, 0, 255 });
	ImageDrawPixel(&icon,34,56,CLITERAL(Color){ 88, 32, 12, 251 });
	ImageDrawPixel(&icon,34,57,CLITERAL(Color){ 170, 185, 208, 57 });
	ImageDrawPixel(&icon,34,58,CLITERAL(Color){ 236, 229, 254, 3 });
	ImageDrawPixel(&icon,35,0,CLITERAL(Color){ 191, 186, 204, 54 });
	ImageDrawPixel(&icon,35,1,CLITERAL(Color){ 34, 57, 0, 255 });
	ImageDrawPixel(&icon,35,2,CLITERAL(Color){ 94, 132, 0, 255 });
	ImageDrawPixel(&icon,35,3,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,35,4,CLITERAL(Color){ 89, 121, 0, 255 });
	ImageDrawPixel(&icon,35,5,CLITERAL(Color){ 99, 133, 1, 255 });
	ImageDrawPixel(&icon,35,6,CLITERAL(Color){ 37, 51, 4, 255 });
	ImageDrawPixel(&icon,35,7,CLITERAL(Color){ 0, 0, 3, 255 });
	ImageDrawPixel(&icon,35,8,CLITERAL(Color){ 1, 1, 4, 255 });
	ImageDrawPixel(&icon,35,9,CLITERAL(Color){ 74, 101, 2, 255 });
	ImageDrawPixel(&icon,35,10,CLITERAL(Color){ 98, 133, 0, 255 });
	ImageDrawPixel(&icon,35,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,35,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,35,13,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,35,14,CLITERAL(Color){ 88, 136, 0, 255 });
	ImageDrawPixel(&icon,35,15,CLITERAL(Color){ 60, 65, 4, 255 });
	ImageDrawPixel(&icon,35,16,CLITERAL(Color){ 161, 30, 4, 255 });
	ImageDrawPixel(&icon,35,17,CLITERAL(Color){ 136, 42, 3, 255 });
	ImageDrawPixel(&icon,35,18,CLITERAL(Color){ 69, 29, 11, 255 });
	ImageDrawPixel(&icon,35,19,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,35,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,25,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,35,26,CLITERAL(Color){ 91, 34, 11, 255 });
	ImageDrawPixel(&icon,35,27,CLITERAL(Color){ 62, 24, 8, 255 });
	ImageDrawPixel(&icon,35,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,43,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,35,44,CLITERAL(Color){ 70, 26, 9, 255 });
	ImageDrawPixel(&icon,35,45,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,35,46,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,48,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,35,52,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,35,53,CLITERAL(Color){ 73, 31, 14, 255 });
	ImageDrawPixel(&icon,35,54,CLITERAL(Color){ 102, 34, 6, 255 });
	ImageDrawPixel(&icon,35,55,CLITERAL(Color){ 165, 22, 0, 255 });
	ImageDrawPixel(&icon,35,56,CLITERAL(Color){ 101, 65, 49, 217 });
	ImageDrawPixel(&icon,36,1,CLITERAL(Color){ 112, 115, 101, 166 });
	ImageDrawPixel(&icon,36,2,CLITERAL(Color){ 30, 65, 0, 255 });
	ImageDrawPixel(&icon,36,3,CLITERAL(Color){ 100, 136, 0, 255 });
	ImageDrawPixel(&icon,36,4,CLITERAL(Color){ 90, 121, 0, 255 });
	ImageDrawPixel(&icon,36,5,CLITERAL(Color){ 99, 134, 1, 255 });
	ImageDrawPixel(&icon,36,6,CLITERAL(Color){ 13, 16, 4, 255 });
	ImageDrawPixel(&icon,36,7,CLITERAL(Color){ 0, 0, 0, 255 });
	ImageDrawPixel(&icon,36,8,CLITERAL(Color){ 0, 0, 2, 255 });
	ImageDrawPixel(&icon,36,9,CLITERAL(Color){ 60, 81, 3, 255 });
	ImageDrawPixel(&icon,36,10,CLITERAL(Color){ 104, 140, 0, 255 });
	ImageDrawPixel(&icon,36,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,36,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,36,13,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,36,14,CLITERAL(Color){ 96, 135, 0, 255 });
	ImageDrawPixel(&icon,36,15,CLITERAL(Color){ 56, 90, 4, 255 });
	ImageDrawPixel(&icon,36,16,CLITERAL(Color){ 130, 30, 5, 255 });
	ImageDrawPixel(&icon,36,17,CLITERAL(Color){ 173, 48, 1, 255 });
	ImageDrawPixel(&icon,36,18,CLITERAL(Color){ 73, 29, 10, 255 });
	ImageDrawPixel(&icon,36,19,CLITERAL(Color){ 93, 36, 13, 255 });
	ImageDrawPixel(&icon,36,20,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,22,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,23,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,24,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,25,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,36,26,CLITERAL(Color){ 91, 35, 12, 255 });
	ImageDrawPixel(&icon,36,27,CLITERAL(Color){ 42, 19, 9, 255 });
	ImageDrawPixel(&icon,36,28,CLITERAL(Color){ 92, 35, 12, 255 });
	ImageDrawPixel(&icon,36,29,CLITERAL(Color){ 107, 39, 12, 255 });
	ImageDrawPixel(&icon,36,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,40,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,36,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,42,CLITERAL(Color){ 108, 40, 12, 255 });
	ImageDrawPixel(&icon,36,43,CLITERAL(Color){ 91, 35, 12, 255 });
	ImageDrawPixel(&icon,36,44,CLITERAL(Color){ 48, 21, 11, 255 });
	ImageDrawPixel(&icon,36,45,CLITERAL(Color){ 91, 35, 12, 255 });
	ImageDrawPixel(&icon,36,46,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,36,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,48,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,36,49,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,51,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,36,52,CLITERAL(Color){ 106, 39, 13, 255 });
	ImageDrawPixel(&icon,36,53,CLITERAL(Color){ 63, 28, 12, 255 });
	ImageDrawPixel(&icon,36,54,CLITERAL(Color){ 128, 41, 3, 255 });
	ImageDrawPixel(&icon,36,55,CLITERAL(Color){ 135, 5, 0, 255 });
	ImageDrawPixel(&icon,36,56,CLITERAL(Color){ 133, 121, 115, 150 });
	ImageDrawPixel(&icon,37,1,CLITERAL(Color){ 242, 237, 255, 2 });
	ImageDrawPixel(&icon,37,2,CLITERAL(Color){ 72, 79, 63, 204 });
	ImageDrawPixel(&icon,37,3,CLITERAL(Color){ 38, 75, 0, 255 });
	ImageDrawPixel(&icon,37,4,CLITERAL(Color){ 97, 134, 0, 255 });
	ImageDrawPixel(&icon,37,5,CLITERAL(Color){ 100, 134, 0, 255 });
	ImageDrawPixel(&icon,37,6,CLITERAL(Color){ 72, 98, 3, 255 });
	ImageDrawPixel(&icon,37,7,CLITERAL(Color){ 24, 33, 4, 255 });
	ImageDrawPixel(&icon,37,8,CLITERAL(Color){ 39, 54, 3, 255 });
	ImageDrawPixel(&icon,37,9,CLITERAL(Color){ 93, 127, 1, 255 });
	ImageDrawPixel(&icon,37,10,CLITERAL(Color){ 90, 122, 0, 255 });
	ImageDrawPixel(&icon,37,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,37,12,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,37,13,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,37,14,CLITERAL(Color){ 93, 124, 0, 255 });
	ImageDrawPixel(&icon,37,15,CLITERAL(Color){ 61, 117, 1, 255 });
	ImageDrawPixel(&icon,37,16,CLITERAL(Color){ 91, 43, 2, 255 });
	ImageDrawPixel(&icon,37,17,CLITERAL(Color){ 202, 47, 1, 255 });
	ImageDrawPixel(&icon,37,18,CLITERAL(Color){ 98, 35, 7, 255 });
	ImageDrawPixel(&icon,37,19,CLITERAL(Color){ 66, 29, 13, 255 });
	ImageDrawPixel(&icon,37,20,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,37,21,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,22,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,37,23,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,37,24,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,37,25,CLITERAL(Color){ 81, 31, 11, 255 });
	ImageDrawPixel(&icon,37,26,CLITERAL(Color){ 67, 28, 11, 255 });
	ImageDrawPixel(&icon,37,27,CLITERAL(Color){ 66, 27, 11, 255 });
	ImageDrawPixel(&icon,37,28,CLITERAL(Color){ 64, 26, 11, 255 });
	ImageDrawPixel(&icon,37,29,CLITERAL(Color){ 76, 30, 11, 255 });
	ImageDrawPixel(&icon,37,30,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,37,31,CLITERAL(Color){ 104, 39, 12, 255 });
	ImageDrawPixel(&icon,37,32,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,37,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,34,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,37,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,39,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,37,40,CLITERAL(Color){ 104, 39, 12, 255 });
	ImageDrawPixel(&icon,37,41,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,37,42,CLITERAL(Color){ 74, 29, 10, 255 });
	ImageDrawPixel(&icon,37,43,CLITERAL(Color){ 63, 26, 11, 255 });
	ImageDrawPixel(&icon,37,44,CLITERAL(Color){ 71, 30, 12, 255 });
	ImageDrawPixel(&icon,37,45,CLITERAL(Color){ 67, 27, 11, 255 });
	ImageDrawPixel(&icon,37,46,CLITERAL(Color){ 77, 30, 11, 255 });
	ImageDrawPixel(&icon,37,47,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,37,48,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,37,49,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,37,50,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,37,51,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,37,52,CLITERAL(Color){ 90, 35, 13, 255 });
	ImageDrawPixel(&icon,37,53,CLITERAL(Color){ 74, 29, 10, 255 });
	ImageDrawPixel(&icon,37,54,CLITERAL(Color){ 180, 50, 0, 255 });
	ImageDrawPixel(&icon,37,55,CLITERAL(Color){ 90, 5, 0, 255 });
	ImageDrawPixel(&icon,37,56,CLITERAL(Color){ 163, 182, 190, 64 });
	ImageDrawPixel(&icon,38,2,CLITERAL(Color){ 247, 240, 255, 2 });
	ImageDrawPixel(&icon,38,3,CLITERAL(Color){ 87, 92, 86, 182 });
	ImageDrawPixel(&icon,38,4,CLITERAL(Color){ 28, 57, 0, 255 });
	ImageDrawPixel(&icon,38,5,CLITERAL(Color){ 66, 104, 0, 255 });
	ImageDrawPixel(&icon,38,6,CLITERAL(Color){ 107, 146, 0, 255 });
	ImageDrawPixel(&icon,38,7,CLITERAL(Color){ 101, 136, 1, 255 });
	ImageDrawPixel(&icon,38,8,CLITERAL(Color){ 99, 134, 0, 255 });
	ImageDrawPixel(&icon,38,9,CLITERAL(Color){ 90, 122, 0, 255 });
	ImageDrawPixel(&icon,38,10,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,38,11,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,38,12,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,38,13,CLITERAL(Color){ 90, 121, 0, 255 });
	ImageDrawPixel(&icon,38,14,CLITERAL(Color){ 100, 135, 0, 255 });
	ImageDrawPixel(&icon,38,15,CLITERAL(Color){ 62, 116, 0, 255 });
	ImageDrawPixel(&icon,38,16,CLITERAL(Color){ 20, 25, 0, 255 });
	ImageDrawPixel(&icon,38,17,CLITERAL(Color){ 166, 31, 0, 255 });
	ImageDrawPixel(&icon,38,18,CLITERAL(Color){ 154, 48, 2, 255 });
	ImageDrawPixel(&icon,38,19,CLITERAL(Color){ 64, 27, 10, 255 });
	ImageDrawPixel(&icon,38,20,CLITERAL(Color){ 98, 37, 13, 255 });
	ImageDrawPixel(&icon,38,21,CLITERAL(Color){ 98, 37, 12, 255 });
	ImageDrawPixel(&icon,38,22,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,38,23,CLITERAL(Color){ 87, 34, 12, 255 });
	ImageDrawPixel(&icon,38,24,CLITERAL(Color){ 65, 26, 10, 255 });
	ImageDrawPixel(&icon,38,25,CLITERAL(Color){ 66, 27, 11, 255 });
	ImageDrawPixel(&icon,38,26,CLITERAL(Color){ 92, 35, 12, 255 });
	ImageDrawPixel(&icon,38,27,CLITERAL(Color){ 112, 41, 13, 255 });
	ImageDrawPixel(&icon,38,28,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,38,29,CLITERAL(Color){ 65, 26, 11, 255 });
	ImageDrawPixel(&icon,38,30,CLITERAL(Color){ 61, 25, 10, 255 });
	ImageDrawPixel(&icon,38,31,CLITERAL(Color){ 83, 32, 11, 255 });
	ImageDrawPixel(&icon,38,32,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,38,33,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,38,34,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,38,35,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,38,36,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,38,37,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,38,38,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,38,39,CLITERAL(Color){ 106, 39, 13, 255 });
	ImageDrawPixel(&icon,38,40,CLITERAL(Color){ 82, 32, 11, 255 });
	ImageDrawPixel(&icon,38,41,CLITERAL(Color){ 62, 25, 10, 255 });
	ImageDrawPixel(&icon,38,42,CLITERAL(Color){ 65, 26, 11, 255 });
	ImageDrawPixel(&icon,38,43,CLITERAL(Color){ 93, 35, 12, 255 });
	ImageDrawPixel(&icon,38,44,CLITERAL(Color){ 112, 41, 12, 255 });
	ImageDrawPixel(&icon,38,45,CLITERAL(Color){ 94, 35, 11, 255 });
	ImageDrawPixel(&icon,38,46,CLITERAL(Color){ 69, 27, 11, 255 });
	ImageDrawPixel(&icon,38,47,CLITERAL(Color){ 64, 26, 10, 255 });
	ImageDrawPixel(&icon,38,48,CLITERAL(Color){ 84, 32, 12, 255 });
	ImageDrawPixel(&icon,38,49,CLITERAL(Color){ 105, 39, 13, 255 });
	ImageDrawPixel(&icon,38,50,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,38,51,CLITERAL(Color){ 105, 38, 12, 255 });
	ImageDrawPixel(&icon,38,52,CLITERAL(Color){ 62, 28, 13, 255 });
	ImageDrawPixel(&icon,38,53,CLITERAL(Color){ 108, 37, 5, 255 });
	ImageDrawPixel(&icon,38,54,CLITERAL(Color){ 177, 24, 0, 255 });
	ImageDrawPixel(&icon,38,55,CLITERAL(Color){ 98, 61, 40, 228 });
	ImageDrawPixel(&icon,39,4,CLITERAL(Color){ 149, 146, 163, 105 });
	ImageDrawPixel(&icon,39,5,CLITERAL(Color){ 63, 76, 44, 222 });
	ImageDrawPixel(&icon,39,6,CLITERAL(Color){ 34, 61, 0, 255 });
	ImageDrawPixel(&icon,39,7,CLITERAL(Color){ 51, 86, 0, 255 });
	ImageDrawPixel(&icon,39,8,CLITERAL(Color){ 72, 109, 0, 255 });
	ImageDrawPixel(&icon,39,9,CLITERAL(Color){ 82, 118, 0, 255 });
	ImageDrawPixel(&icon,39,10,CLITERAL(Color){ 86, 122, 0, 255 });
	ImageDrawPixel(&icon,39,11,CLITERAL(Color){ 87, 123, 0, 255 });
	ImageDrawPixel(&icon,39,12,CLITERAL(Color){ 79, 117, 0, 255 });
	ImageDrawPixel(&icon,39,13,CLITERAL(Color){ 57, 93, 0, 255 });
	ImageDrawPixel(&icon,39,14,CLITERAL(Color){ 31, 60, 0, 255 });
	ImageDrawPixel(&icon,39,15,CLITERAL(Color){ 91, 103, 61, 207 });
	ImageDrawPixel(&icon,39,16,CLITERAL(Color){ 128, 143, 150, 122 });
	ImageDrawPixel(&icon,39,17,CLITERAL(Color){ 100, 29, 2, 255 });
	ImageDrawPixel(&icon,39,18,CLITERAL(Color){ 203, 49, 0, 255 });
	ImageDrawPixel(&icon,39,19,CLITERAL(Color){ 99, 35, 7, 255 });
	ImageDrawPixel(&icon,39,20,CLITERAL(Color){ 68, 30, 14, 255 });
	ImageDrawPixel(&icon,39,21,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,39,22,CLITERAL(Color){ 69, 28, 11, 255 });
	ImageDrawPixel(&icon,39,23,CLITERAL(Color){ 65, 27, 11, 255 });
	ImageDrawPixel(&icon,39,24,CLITERAL(Color){ 83, 32, 11, 255 });
	ImageDrawPixel(&icon,39,25,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,39,26,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,39,27,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,39,28,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,39,29,CLITERAL(Color){ 106, 39, 13, 255 });
	ImageDrawPixel(&icon,39,30,CLITERAL(Color){ 85, 33, 11, 255 });
	ImageDrawPixel(&icon,39,31,CLITERAL(Color){ 61, 25, 10, 255 });
	ImageDrawPixel(&icon,39,32,CLITERAL(Color){ 63, 26, 11, 255 });
	ImageDrawPixel(&icon,39,33,CLITERAL(Color){ 90, 34, 12, 255 });
	ImageDrawPixel(&icon,39,34,CLITERAL(Color){ 108, 40, 13, 255 });
	ImageDrawPixel(&icon,39,35,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,39,36,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,39,37,CLITERAL(Color){ 108, 40, 13, 255 });
	ImageDrawPixel(&icon,39,38,CLITERAL(Color){ 90, 34, 12, 255 });
	ImageDrawPixel(&icon,39,39,CLITERAL(Color){ 63, 26, 10, 255 });
	ImageDrawPixel(&icon,39,40,CLITERAL(Color){ 62, 25, 10, 255 });
	ImageDrawPixel(&icon,39,41,CLITERAL(Color){ 87, 33, 11, 255 });
	ImageDrawPixel(&icon,39,42,CLITERAL(Color){ 107, 39, 13, 255 });
	ImageDrawPixel(&icon,39,43,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,39,44,CLITERAL(Color){ 94, 35, 12, 255 });
	ImageDrawPixel(&icon,39,45,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,39,46,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,39,47,CLITERAL(Color){ 87, 33, 11, 255 });
	ImageDrawPixel(&icon,39,48,CLITERAL(Color){ 66, 27, 11, 255 });
	ImageDrawPixel(&icon,39,49,CLITERAL(Color){ 67, 27, 11, 255 });
	ImageDrawPixel(&icon,39,50,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,39,51,CLITERAL(Color){ 91, 36, 14, 255 });
	ImageDrawPixel(&icon,39,52,CLITERAL(Color){ 70, 28, 10, 255 });
	ImageDrawPixel(&icon,39,53,CLITERAL(Color){ 188, 55, 0, 255 });
	ImageDrawPixel(&icon,39,54,CLITERAL(Color){ 93, 0, 0, 255 });
	ImageDrawPixel(&icon,39,55,CLITERAL(Color){ 149, 159, 163, 104 });
	ImageDrawPixel(&icon,40,5,CLITERAL(Color){ 246, 240, 255, 2 });
	ImageDrawPixel(&icon,40,6,CLITERAL(Color){ 157, 154, 171, 96 });
	ImageDrawPixel(&icon,40,7,CLITERAL(Color){ 115, 122, 100, 163 });
	ImageDrawPixel(&icon,40,8,CLITERAL(Color){ 93, 108, 53, 210 });
	ImageDrawPixel(&icon,40,9,CLITERAL(Color){ 74, 92, 22, 240 });
	ImageDrawPixel(&icon,40,10,CLITERAL(Color){ 63, 84, 5, 255 });
	ImageDrawPixel(&icon,40,11,CLITERAL(Color){ 63, 85, 5, 255 });
	ImageDrawPixel(&icon,40,12,CLITERAL(Color){ 80, 97, 32, 231 });
	ImageDrawPixel(&icon,40,13,CLITERAL(Color){ 100, 110, 80, 184 });
	ImageDrawPixel(&icon,40,14,CLITERAL(Color){ 148, 146, 160, 107 });
	ImageDrawPixel(&icon,40,17,CLITERAL(Color){ 103, 98, 93, 175 });
	ImageDrawPixel(&icon,40,18,CLITERAL(Color){ 127, 0, 0, 255 });
	ImageDrawPixel(&icon,40,19,CLITERAL(Color){ 184, 56, 2, 255 });
	ImageDrawPixel(&icon,40,20,CLITERAL(Color){ 64, 28, 10, 255 });
	ImageDrawPixel(&icon,40,21,CLITERAL(Color){ 51, 24, 13, 255 });
	ImageDrawPixel(&icon,40,22,CLITERAL(Color){ 80, 31, 11, 255 });
	ImageDrawPixel(&icon,40,23,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,40,24,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,40,25,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,40,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,40,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,40,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,40,29,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,40,30,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,40,31,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,40,32,CLITERAL(Color){ 74, 29, 11, 255 });
	ImageDrawPixel(&icon,40,33,CLITERAL(Color){ 60, 25, 10, 255 });
	ImageDrawPixel(&icon,40,34,CLITERAL(Color){ 70, 28, 11, 255 });
	ImageDrawPixel(&icon,40,35,CLITERAL(Color){ 102, 37, 12, 255 });
	ImageDrawPixel(&icon,40,36,CLITERAL(Color){ 100, 37, 12, 255 });
	ImageDrawPixel(&icon,40,37,CLITERAL(Color){ 69, 27, 11, 255 });
	ImageDrawPixel(&icon,40,38,CLITERAL(Color){ 61, 25, 11, 255 });
	ImageDrawPixel(&icon,40,39,CLITERAL(Color){ 76, 29, 11, 255 });
	ImageDrawPixel(&icon,40,40,CLITERAL(Color){ 105, 39, 12, 255 });
	ImageDrawPixel(&icon,40,41,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,40,42,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,40,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,40,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,40,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,40,46,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,40,47,CLITERAL(Color){ 102, 38, 12, 255 });
	ImageDrawPixel(&icon,40,48,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,40,49,CLITERAL(Color){ 78, 30, 11, 255 });
	ImageDrawPixel(&icon,40,50,CLITERAL(Color){ 66, 27, 12, 255 });
	ImageDrawPixel(&icon,40,51,CLITERAL(Color){ 47, 24, 13, 255 });
	ImageDrawPixel(&icon,40,52,CLITERAL(Color){ 141, 47, 5, 255 });
	ImageDrawPixel(&icon,40,53,CLITERAL(Color){ 173, 20, 0, 255 });
	ImageDrawPixel(&icon,40,54,CLITERAL(Color){ 84, 52, 35, 234 });
	ImageDrawPixel(&icon,40,55,CLITERAL(Color){ 232, 249, 255, 4 });
	ImageDrawPixel(&icon,41,9,CLITERAL(Color){ 229, 224, 244, 12 });
	ImageDrawPixel(&icon,41,10,CLITERAL(Color){ 214, 211, 227, 40 });
	ImageDrawPixel(&icon,41,11,CLITERAL(Color){ 215, 211, 227, 37 });
	ImageDrawPixel(&icon,41,12,CLITERAL(Color){ 236, 230, 253, 5 });
	ImageDrawPixel(&icon,41,17,CLITERAL(Color){ 215, 235, 242, 17 });
	ImageDrawPixel(&icon,41,18,CLITERAL(Color){ 65, 20, 8, 254 });
	ImageDrawPixel(&icon,41,19,CLITERAL(Color){ 183, 29, 0, 255 });
	ImageDrawPixel(&icon,41,20,CLITERAL(Color){ 146, 47, 4, 255 });
	ImageDrawPixel(&icon,41,21,CLITERAL(Color){ 51, 25, 12, 255 });
	ImageDrawPixel(&icon,41,22,CLITERAL(Color){ 98, 38, 14, 255 });
	ImageDrawPixel(&icon,41,23,CLITERAL(Color){ 103, 38, 12, 255 });
	ImageDrawPixel(&icon,41,24,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,41,25,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,32,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,41,33,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,41,34,CLITERAL(Color){ 69, 28, 11, 255 });
	ImageDrawPixel(&icon,41,35,CLITERAL(Color){ 63, 26, 12, 255 });
	ImageDrawPixel(&icon,41,36,CLITERAL(Color){ 63, 26, 11, 255 });
	ImageDrawPixel(&icon,41,37,CLITERAL(Color){ 70, 27, 11, 255 });
	ImageDrawPixel(&icon,41,38,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,41,39,CLITERAL(Color){ 106, 39, 12, 255 });
	ImageDrawPixel(&icon,41,40,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,41,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,46,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,47,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,41,48,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,41,49,CLITERAL(Color){ 111, 40, 13, 255 });
	ImageDrawPixel(&icon,41,50,CLITERAL(Color){ 57, 27, 14, 255 });
	ImageDrawPixel(&icon,41,51,CLITERAL(Color){ 97, 35, 7, 255 });
	ImageDrawPixel(&icon,41,52,CLITERAL(Color){ 207, 51, 0, 255 });
	ImageDrawPixel(&icon,41,53,CLITERAL(Color){ 78, 0, 0, 255 });
	ImageDrawPixel(&icon,41,54,CLITERAL(Color){ 176, 195, 200, 68 });
	ImageDrawPixel(&icon,42,18,CLITERAL(Color){ 161, 181, 184, 81 });
	ImageDrawPixel(&icon,42,19,CLITERAL(Color){ 68, 0, 0, 255 });
	ImageDrawPixel(&icon,42,20,CLITERAL(Color){ 204, 45, 0, 255 });
	ImageDrawPixel(&icon,42,21,CLITERAL(Color){ 138, 46, 4, 255 });
	ImageDrawPixel(&icon,42,22,CLITERAL(Color){ 52, 26, 13, 255 });
	ImageDrawPixel(&icon,42,23,CLITERAL(Color){ 89, 34, 13, 255 });
	ImageDrawPixel(&icon,42,24,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,42,25,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,42,26,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,32,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,42,33,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,42,34,CLITERAL(Color){ 113, 42, 13, 255 });
	ImageDrawPixel(&icon,42,35,CLITERAL(Color){ 68, 27, 10, 255 });
	ImageDrawPixel(&icon,42,36,CLITERAL(Color){ 70, 27, 10, 255 });
	ImageDrawPixel(&icon,42,37,CLITERAL(Color){ 113, 42, 13, 255 });
	ImageDrawPixel(&icon,42,38,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,42,39,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,42,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,45,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,46,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,42,47,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,42,48,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,42,49,CLITERAL(Color){ 60, 27, 14, 255 });
	ImageDrawPixel(&icon,42,50,CLITERAL(Color){ 98, 35, 8, 255 });
	ImageDrawPixel(&icon,42,51,CLITERAL(Color){ 205, 57, 0, 255 });
	ImageDrawPixel(&icon,42,52,CLITERAL(Color){ 109, 0, 0, 255 });
	ImageDrawPixel(&icon,42,53,CLITERAL(Color){ 126, 129, 145, 128 });
	ImageDrawPixel(&icon,43,19,CLITERAL(Color){ 126, 139, 137, 131 });
	ImageDrawPixel(&icon,43,20,CLITERAL(Color){ 87, 0, 0, 255 });
	ImageDrawPixel(&icon,43,21,CLITERAL(Color){ 209, 53, 0, 255 });
	ImageDrawPixel(&icon,43,22,CLITERAL(Color){ 138, 43, 3, 255 });
	ImageDrawPixel(&icon,43,23,CLITERAL(Color){ 58, 26, 11, 255 });
	ImageDrawPixel(&icon,43,24,CLITERAL(Color){ 76, 32, 14, 255 });
	ImageDrawPixel(&icon,43,25,CLITERAL(Color){ 105, 39, 12, 255 });
	ImageDrawPixel(&icon,43,26,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,43,27,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,28,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,34,CLITERAL(Color){ 103, 39, 13, 255 });
	ImageDrawPixel(&icon,43,35,CLITERAL(Color){ 78, 29, 10, 255 });
	ImageDrawPixel(&icon,43,36,CLITERAL(Color){ 78, 29, 10, 255 });
	ImageDrawPixel(&icon,43,37,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,43,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,44,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,43,45,CLITERAL(Color){ 95, 36, 12, 255 });
	ImageDrawPixel(&icon,43,46,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,43,47,CLITERAL(Color){ 92, 35, 13, 255 });
	ImageDrawPixel(&icon,43,48,CLITERAL(Color){ 57, 26, 13, 255 });
	ImageDrawPixel(&icon,43,49,CLITERAL(Color){ 101, 36, 7, 255 });
	ImageDrawPixel(&icon,43,50,CLITERAL(Color){ 200, 54, 0, 255 });
	ImageDrawPixel(&icon,43,51,CLITERAL(Color){ 160, 29, 4, 255 });
	ImageDrawPixel(&icon,43,52,CLITERAL(Color){ 31, 42, 3, 255 });
	ImageDrawPixel(&icon,43,53,CLITERAL(Color){ 85, 122, 82, 183 });
	ImageDrawPixel(&icon,43,54,CLITERAL(Color){ 153, 149, 162, 103 });
	ImageDrawPixel(&icon,43,55,CLITERAL(Color){ 228, 221, 248, 13 });
	ImageDrawPixel(&icon,44,20,CLITERAL(Color){ 128, 134, 150, 122 });
	ImageDrawPixel(&icon,44,21,CLITERAL(Color){ 92, 0, 1, 255 });
	ImageDrawPixel(&icon,44,22,CLITERAL(Color){ 209, 51, 0, 255 });
	ImageDrawPixel(&icon,44,23,CLITERAL(Color){ 156, 48, 2, 255 });
	ImageDrawPixel(&icon,44,24,CLITERAL(Color){ 76, 30, 9, 255 });
	ImageDrawPixel(&icon,44,25,CLITERAL(Color){ 61, 28, 13, 255 });
	ImageDrawPixel(&icon,44,26,CLITERAL(Color){ 94, 36, 14, 255 });
	ImageDrawPixel(&icon,44,27,CLITERAL(Color){ 104, 38, 12, 255 });
	ImageDrawPixel(&icon,44,28,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,44,29,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,30,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,31,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,34,CLITERAL(Color){ 103, 39, 13, 255 });
	ImageDrawPixel(&icon,44,35,CLITERAL(Color){ 76, 29, 10, 255 });
	ImageDrawPixel(&icon,44,36,CLITERAL(Color){ 77, 29, 10, 255 });
	ImageDrawPixel(&icon,44,37,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,44,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,41,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,42,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,43,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,44,44,CLITERAL(Color){ 101, 37, 12, 255 });
	ImageDrawPixel(&icon,44,45,CLITERAL(Color){ 102, 38, 13, 255 });
	ImageDrawPixel(&icon,44,46,CLITERAL(Color){ 70, 30, 14, 255 });
	ImageDrawPixel(&icon,44,47,CLITERAL(Color){ 64, 28, 11, 255 });
	ImageDrawPixel(&icon,44,48,CLITERAL(Color){ 124, 41, 4, 255 });
	ImageDrawPixel(&icon,44,49,CLITERAL(Color){ 205, 53, 0, 255 });
	ImageDrawPixel(&icon,44,50,CLITERAL(Color){ 153, 25, 6, 255 });
	ImageDrawPixel(&icon,44,51,CLITERAL(Color){ 45, 50, 9, 255 });
	ImageDrawPixel(&icon,44,52,CLITERAL(Color){ 77, 137, 2, 255 });
	ImageDrawPixel(&icon,44,53,CLITERAL(Color){ 77, 114, 0, 255 });
	ImageDrawPixel(&icon,44,54,CLITERAL(Color){ 34, 62, 0, 255 });
	ImageDrawPixel(&icon,44,55,CLITERAL(Color){ 66, 83, 29, 235 });
	ImageDrawPixel(&icon,44,56,CLITERAL(Color){ 113, 118, 108, 158 });
	ImageDrawPixel(&icon,44,57,CLITERAL(Color){ 176, 172, 195, 72 });
	ImageDrawPixel(&icon,45,18,CLITERAL(Color){ 240, 234, 255, 1 });
	ImageDrawPixel(&icon,45,19,CLITERAL(Color){ 179, 173, 191, 76 });
	ImageDrawPixel(&icon,45,20,CLITERAL(Color){ 99, 128, 101, 164 });
	ImageDrawPixel(&icon,45,21,CLITERAL(Color){ 28, 64, 2, 255 });
	ImageDrawPixel(&icon,45,22,CLITERAL(Color){ 101, 27, 7, 255 });
	ImageDrawPixel(&icon,45,23,CLITERAL(Color){ 198, 40, 2, 255 });
	ImageDrawPixel(&icon,45,24,CLITERAL(Color){ 189, 54, 1, 255 });
	ImageDrawPixel(&icon,45,25,CLITERAL(Color){ 107, 37, 6, 255 });
	ImageDrawPixel(&icon,45,26,CLITERAL(Color){ 67, 28, 10, 255 });
	ImageDrawPixel(&icon,45,27,CLITERAL(Color){ 66, 29, 13, 255 });
	ImageDrawPixel(&icon,45,28,CLITERAL(Color){ 89, 35, 13, 255 });
	ImageDrawPixel(&icon,45,29,CLITERAL(Color){ 104, 39, 13, 255 });
	ImageDrawPixel(&icon,45,30,CLITERAL(Color){ 101, 38, 12, 255 });
	ImageDrawPixel(&icon,45,31,CLITERAL(Color){ 97, 36, 12, 255 });
	ImageDrawPixel(&icon,45,32,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,45,33,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,45,34,CLITERAL(Color){ 103, 39, 13, 255 });
	ImageDrawPixel(&icon,45,35,CLITERAL(Color){ 76, 29, 10, 255 });
	ImageDrawPixel(&icon,45,36,CLITERAL(Color){ 77, 29, 10, 255 });
	ImageDrawPixel(&icon,45,37,CLITERAL(Color){ 103, 38, 13, 255 });
	ImageDrawPixel(&icon,45,38,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,45,39,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,45,40,CLITERAL(Color){ 96, 36, 12, 255 });
	ImageDrawPixel(&icon,45,41,CLITERAL(Color){ 99, 37, 12, 255 });
	ImageDrawPixel(&icon,45,42,CLITERAL(Color){ 104, 38, 13, 255 });
	ImageDrawPixel(&icon,45,43,CLITERAL(Color){ 97, 37, 13, 255 });
	ImageDrawPixel(&icon,45,44,CLITERAL(Color){ 74, 31, 14, 255 });
	ImageDrawPixel(&icon,45,45,CLITERAL(Color){ 64, 28, 12, 255 });
	ImageDrawPixel(&icon,45,46,CLITERAL(Color){ 88, 32, 7, 255 });
	ImageDrawPixel(&icon,45,47,CLITERAL(Color){ 161, 47, 1, 255 });
	ImageDrawPixel(&icon,45,48,CLITERAL(Color){ 210, 50, 0, 255 });
	ImageDrawPixel(&icon,45,49,CLITERAL(Color){ 130, 25, 6, 255 });
	ImageDrawPixel(&icon,45,50,CLITERAL(Color){ 43, 60, 9, 255 });
	ImageDrawPixel(&icon,45,51,CLITERAL(Color){ 73, 131, 2, 255 });
	ImageDrawPixel(&icon,45,52,CLITERAL(Color){ 98, 129, 0, 255 });
	ImageDrawPixel(&icon,45,53,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,45,54,CLITERAL(Color){ 98, 133, 0, 255 });
	ImageDrawPixel(&icon,45,55,CLITERAL(Color){ 81, 120, 0, 255 });
	ImageDrawPixel(&icon,45,56,CLITERAL(Color){ 44, 79, 0, 255 });
	ImageDrawPixel(&icon,45,57,CLITERAL(Color){ 37, 63, 0, 255 });
	ImageDrawPixel(&icon,45,58,CLITERAL(Color){ 77, 87, 72, 195 });
	ImageDrawPixel(&icon,45,59,CLITERAL(Color){ 201, 197, 211, 58 });
	ImageDrawPixel(&icon,46,16,CLITERAL(Color){ 198, 192, 216, 47 });
	ImageDrawPixel(&icon,46,17,CLITERAL(Color){ 122, 124, 123, 144 });
	ImageDrawPixel(&icon,46,18,CLITERAL(Color){ 70, 85, 41, 224 });
	ImageDrawPixel(&icon,46,19,CLITERAL(Color){ 42, 69, 0, 255 });
	ImageDrawPixel(&icon,46,20,CLITERAL(Color){ 64, 97, 0, 255 });
	ImageDrawPixel(&icon,46,21,CLITERAL(Color){ 96, 147, 0, 255 });
	ImageDrawPixel(&icon,46,22,CLITERAL(Color){ 49, 99, 6, 255 });
	ImageDrawPixel(&icon,46,23,CLITERAL(Color){ 72, 37, 9, 255 });
	ImageDrawPixel(&icon,46,24,CLITERAL(Color){ 154, 25, 5, 255 });
	ImageDrawPixel(&icon,46,25,CLITERAL(Color){ 208, 51, 0, 255 });
	ImageDrawPixel(&icon,46,26,CLITERAL(Color){ 161, 48, 1, 255 });
	ImageDrawPixel(&icon,46,27,CLITERAL(Color){ 107, 36, 6, 255 });
	ImageDrawPixel(&icon,46,28,CLITERAL(Color){ 74, 29, 9, 255 });
	ImageDrawPixel(&icon,46,29,CLITERAL(Color){ 67, 28, 11, 255 });
	ImageDrawPixel(&icon,46,30,CLITERAL(Color){ 70, 30, 13, 255 });
	ImageDrawPixel(&icon,46,31,CLITERAL(Color){ 84, 33, 13, 255 });
	ImageDrawPixel(&icon,46,32,CLITERAL(Color){ 97, 37, 13, 255 });
	ImageDrawPixel(&icon,46,33,CLITERAL(Color){ 101, 38, 13, 255 });
	ImageDrawPixel(&icon,46,34,CLITERAL(Color){ 109, 41, 14, 255 });
	ImageDrawPixel(&icon,46,35,CLITERAL(Color){ 79, 30, 10, 255 });
	ImageDrawPixel(&icon,46,36,CLITERAL(Color){ 80, 30, 10, 255 });
	ImageDrawPixel(&icon,46,37,CLITERAL(Color){ 109, 40, 14, 255 });
	ImageDrawPixel(&icon,46,38,CLITERAL(Color){ 100, 38, 13, 255 });
	ImageDrawPixel(&icon,46,39,CLITERAL(Color){ 99, 38, 13, 255 });
	ImageDrawPixel(&icon,46,40,CLITERAL(Color){ 90, 35, 13, 255 });
	ImageDrawPixel(&icon,46,41,CLITERAL(Color){ 74, 31, 13, 255 });
	ImageDrawPixel(&icon,46,42,CLITERAL(Color){ 69, 29, 13, 255 });
	ImageDrawPixel(&icon,46,43,CLITERAL(Color){ 70, 28, 11, 255 });
	ImageDrawPixel(&icon,46,44,CLITERAL(Color){ 93, 33, 7, 255 });
	ImageDrawPixel(&icon,46,45,CLITERAL(Color){ 141, 45, 4, 255 });
	ImageDrawPixel(&icon,46,46,CLITERAL(Color){ 199, 51, 0, 255 });
	ImageDrawPixel(&icon,46,47,CLITERAL(Color){ 159, 19, 0, 255 });
	ImageDrawPixel(&icon,46,48,CLITERAL(Color){ 60, 0, 0, 255 });
	ImageDrawPixel(&icon,46,49,CLITERAL(Color){ 32, 72, 0, 255 });
	ImageDrawPixel(&icon,46,50,CLITERAL(Color){ 90, 147, 0, 255 });
	ImageDrawPixel(&icon,46,51,CLITERAL(Color){ 96, 125, 0, 255 });
	ImageDrawPixel(&icon,46,52,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,46,53,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,46,54,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,46,55,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,46,56,CLITERAL(Color){ 94, 126, 0, 255 });
	ImageDrawPixel(&icon,46,57,CLITERAL(Color){ 97, 134, 0, 255 });
	ImageDrawPixel(&icon,46,58,CLITERAL(Color){ 52, 90, 0, 255 });
	ImageDrawPixel(&icon,46,59,CLITERAL(Color){ 26, 47, 0, 255 });
	ImageDrawPixel(&icon,46,60,CLITERAL(Color){ 182, 179, 192, 78 });
	ImageDrawPixel(&icon,47,14,CLITERAL(Color){ 183, 178, 196, 70 });
	ImageDrawPixel(&icon,47,15,CLITERAL(Color){ 83, 93, 75, 192 });
	ImageDrawPixel(&icon,47,16,CLITERAL(Color){ 48, 72, 0, 255 });
	ImageDrawPixel(&icon,47,17,CLITERAL(Color){ 41, 73, 0, 255 });
	ImageDrawPixel(&icon,47,18,CLITERAL(Color){ 74, 111, 0, 255 });
	ImageDrawPixel(&icon,47,19,CLITERAL(Color){ 96, 132, 0, 255 });
	ImageDrawPixel(&icon,47,20,CLITERAL(Color){ 92, 123, 0, 255 });
	ImageDrawPixel(&icon,47,21,CLITERAL(Color){ 89, 117, 0, 255 });
	ImageDrawPixel(&icon,47,22,CLITERAL(Color){ 97, 137, 0, 255 });
	ImageDrawPixel(&icon,47,23,CLITERAL(Color){ 64, 122, 2, 255 });
	ImageDrawPixel(&icon,47,24,CLITERAL(Color){ 50, 66, 8, 255 });
	ImageDrawPixel(&icon,47,25,CLITERAL(Color){ 100, 33, 8, 255 });
	ImageDrawPixel(&icon,47,26,CLITERAL(Color){ 163, 31, 0, 255 });
	ImageDrawPixel(&icon,47,27,CLITERAL(Color){ 195, 41, 0, 255 });
	ImageDrawPixel(&icon,47,28,CLITERAL(Color){ 173, 50, 0, 255 });
	ImageDrawPixel(&icon,47,29,CLITERAL(Color){ 128, 40, 4, 255 });
	ImageDrawPixel(&icon,47,30,CLITERAL(Color){ 104, 35, 6, 255 });
	ImageDrawPixel(&icon,47,31,CLITERAL(Color){ 82, 30, 8, 255 });
	ImageDrawPixel(&icon,47,32,CLITERAL(Color){ 74, 28, 9, 255 });
	ImageDrawPixel(&icon,47,33,CLITERAL(Color){ 74, 29, 10, 255 });
	ImageDrawPixel(&icon,47,34,CLITERAL(Color){ 79, 31, 12, 255 });
	ImageDrawPixel(&icon,47,35,CLITERAL(Color){ 55, 24, 10, 255 });
	ImageDrawPixel(&icon,47,36,CLITERAL(Color){ 57, 25, 11, 255 });
	ImageDrawPixel(&icon,47,37,CLITERAL(Color){ 77, 30, 12, 255 });
	ImageDrawPixel(&icon,47,38,CLITERAL(Color){ 75, 29, 10, 255 });
	ImageDrawPixel(&icon,47,39,CLITERAL(Color){ 72, 27, 9, 255 });
	ImageDrawPixel(&icon,47,40,CLITERAL(Color){ 78, 29, 9, 255 });
	ImageDrawPixel(&icon,47,41,CLITERAL(Color){ 97, 34, 7, 255 });
	ImageDrawPixel(&icon,47,42,CLITERAL(Color){ 120, 38, 5, 255 });
	ImageDrawPixel(&icon,47,43,CLITERAL(Color){ 158, 47, 2, 255 });
	ImageDrawPixel(&icon,47,44,CLITERAL(Color){ 197, 47, 0, 255 });
	ImageDrawPixel(&icon,47,45,CLITERAL(Color){ 155, 15, 0, 255 });
	ImageDrawPixel(&icon,47,46,CLITERAL(Color){ 85, 2, 0, 255 });
	ImageDrawPixel(&icon,47,47,CLITERAL(Color){ 118, 101, 96, 175 });
	ImageDrawPixel(&icon,47,48,CLITERAL(Color){ 148, 171, 182, 89 });
	ImageDrawPixel(&icon,47,49,CLITERAL(Color){ 60, 89, 27, 240 });
	ImageDrawPixel(&icon,47,50,CLITERAL(Color){ 61, 97, 0, 255 });
	ImageDrawPixel(&icon,47,51,CLITERAL(Color){ 99, 132, 0, 255 });
	ImageDrawPixel(&icon,47,52,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,47,53,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,47,54,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,47,55,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,47,56,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,47,57,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,47,58,CLITERAL(Color){ 97, 129, 0, 255 });
	ImageDrawPixel(&icon,47,59,CLITERAL(Color){ 64, 104, 0, 255 });
	ImageDrawPixel(&icon,47,60,CLITERAL(Color){ 57, 74, 19, 252 });
	ImageDrawPixel(&icon,47,61,CLITERAL(Color){ 237, 233, 246, 14 });
	ImageDrawPixel(&icon,48,13,CLITERAL(Color){ 133, 134, 134, 134 });
	ImageDrawPixel(&icon,48,14,CLITERAL(Color){ 26, 50, 0, 255 });
	ImageDrawPixel(&icon,48,15,CLITERAL(Color){ 54, 92, 0, 255 });
	ImageDrawPixel(&icon,48,16,CLITERAL(Color){ 93, 130, 0, 255 });
	ImageDrawPixel(&icon,48,17,CLITERAL(Color){ 95, 128, 0, 255 });
	ImageDrawPixel(&icon,48,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,48,19,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,48,20,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,48,21,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,48,22,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,48,23,CLITERAL(Color){ 96, 127, 0, 255 });
	ImageDrawPixel(&icon,48,24,CLITERAL(Color){ 95, 146, 0, 255 });
	ImageDrawPixel(&icon,48,25,CLITERAL(Color){ 38, 97, 0, 255 });
	ImageDrawPixel(&icon,48,26,CLITERAL(Color){ 36, 32, 7, 255 });
	ImageDrawPixel(&icon,48,27,CLITERAL(Color){ 103, 36, 22, 246 });
	ImageDrawPixel(&icon,48,28,CLITERAL(Color){ 111, 5, 0, 255 });
	ImageDrawPixel(&icon,48,29,CLITERAL(Color){ 157, 18, 0, 255 });
	ImageDrawPixel(&icon,48,30,CLITERAL(Color){ 190, 40, 0, 255 });
	ImageDrawPixel(&icon,48,31,CLITERAL(Color){ 178, 46, 0, 255 });
	ImageDrawPixel(&icon,48,32,CLITERAL(Color){ 157, 44, 0, 255 });
	ImageDrawPixel(&icon,48,33,CLITERAL(Color){ 144, 43, 2, 255 });
	ImageDrawPixel(&icon,48,34,CLITERAL(Color){ 133, 40, 3, 255 });
	ImageDrawPixel(&icon,48,35,CLITERAL(Color){ 130, 41, 4, 255 });
	ImageDrawPixel(&icon,48,36,CLITERAL(Color){ 130, 42, 4, 255 });
	ImageDrawPixel(&icon,48,37,CLITERAL(Color){ 130, 39, 3, 255 });
	ImageDrawPixel(&icon,48,38,CLITERAL(Color){ 140, 42, 3, 255 });
	ImageDrawPixel(&icon,48,39,CLITERAL(Color){ 152, 44, 1, 255 });
	ImageDrawPixel(&icon,48,40,CLITERAL(Color){ 170, 45, 0, 255 });
	ImageDrawPixel(&icon,48,41,CLITERAL(Color){ 189, 42, 0, 255 });
	ImageDrawPixel(&icon,48,42,CLITERAL(Color){ 168, 24, 0, 255 });
	ImageDrawPixel(&icon,48,43,CLITERAL(Color){ 119, 3, 0, 255 });
	ImageDrawPixel(&icon,48,44,CLITERAL(Color){ 95, 21, 0, 255 });
	ImageDrawPixel(&icon,48,45,CLITERAL(Color){ 94, 71, 68, 194 });
	ImageDrawPixel(&icon,48,46,CLITERAL(Color){ 157, 175, 186, 82 });
	ImageDrawPixel(&icon,48,49,CLITERAL(Color){ 213, 207, 230, 36 });
	ImageDrawPixel(&icon,48,50,CLITERAL(Color){ 58, 69, 48, 221 });
	ImageDrawPixel(&icon,48,51,CLITERAL(Color){ 42, 79, 0, 255 });
	ImageDrawPixel(&icon,48,52,CLITERAL(Color){ 98, 134, 0, 255 });
	ImageDrawPixel(&icon,48,53,CLITERAL(Color){ 90, 121, 0, 255 });
	ImageDrawPixel(&icon,48,54,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,48,55,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,48,56,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,48,57,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,48,58,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,48,59,CLITERAL(Color){ 100, 136, 0, 255 });
	ImageDrawPixel(&icon,48,60,CLITERAL(Color){ 45, 73, 0, 255 });
	ImageDrawPixel(&icon,48,61,CLITERAL(Color){ 188, 184, 202, 61 });
	ImageDrawPixel(&icon,49,12,CLITERAL(Color){ 155, 154, 161, 105 });
	ImageDrawPixel(&icon,49,13,CLITERAL(Color){ 14, 43, 0, 255 });
	ImageDrawPixel(&icon,49,14,CLITERAL(Color){ 91, 130, 0, 255 });
	ImageDrawPixel(&icon,49,15,CLITERAL(Color){ 95, 126, 0, 255 });
	ImageDrawPixel(&icon,49,16,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,49,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,49,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,49,19,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,49,20,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,49,21,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,49,22,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,49,23,CLITERAL(Color){ 98, 130, 0, 255 });
	ImageDrawPixel(&icon,49,24,CLITERAL(Color){ 72, 108, 0, 255 });
	ImageDrawPixel(&icon,49,25,CLITERAL(Color){ 42, 67, 0, 255 });
	ImageDrawPixel(&icon,49,26,CLITERAL(Color){ 176, 192, 195, 72 });
	ImageDrawPixel(&icon,49,27,CLITERAL(Color){ 208, 228, 242, 21 });
	ImageDrawPixel(&icon,49,28,CLITERAL(Color){ 140, 144, 147, 121 });
	ImageDrawPixel(&icon,49,29,CLITERAL(Color){ 108, 80, 70, 195 });
	ImageDrawPixel(&icon,49,30,CLITERAL(Color){ 109, 45, 19, 245 });
	ImageDrawPixel(&icon,49,31,CLITERAL(Color){ 110, 21, 0, 255 });
	ImageDrawPixel(&icon,49,32,CLITERAL(Color){ 115, 7, 0, 255 });
	ImageDrawPixel(&icon,49,33,CLITERAL(Color){ 126, 6, 0, 255 });
	ImageDrawPixel(&icon,49,34,CLITERAL(Color){ 139, 12, 0, 255 });
	ImageDrawPixel(&icon,49,35,CLITERAL(Color){ 147, 13, 0, 255 });
	ImageDrawPixel(&icon,49,36,CLITERAL(Color){ 152, 18, 0, 255 });
	ImageDrawPixel(&icon,49,37,CLITERAL(Color){ 144, 15, 0, 255 });
	ImageDrawPixel(&icon,49,38,CLITERAL(Color){ 129, 7, 0, 255 });
	ImageDrawPixel(&icon,49,39,CLITERAL(Color){ 117, 5, 0, 255 });
	ImageDrawPixel(&icon,49,40,CLITERAL(Color){ 111, 16, 0, 255 });
	ImageDrawPixel(&icon,49,41,CLITERAL(Color){ 108, 38, 9, 255 });
	ImageDrawPixel(&icon,49,42,CLITERAL(Color){ 107, 70, 57, 209 });
	ImageDrawPixel(&icon,49,43,CLITERAL(Color){ 127, 124, 124, 143 });
	ImageDrawPixel(&icon,49,44,CLITERAL(Color){ 184, 204, 214, 50 });
	ImageDrawPixel(&icon,49,50,CLITERAL(Color){ 243, 236, 255, 8 });
	ImageDrawPixel(&icon,49,51,CLITERAL(Color){ 89, 95, 91, 179 });
	ImageDrawPixel(&icon,49,52,CLITERAL(Color){ 29, 59, 0, 255 });
	ImageDrawPixel(&icon,49,53,CLITERAL(Color){ 76, 115, 0, 255 });
	ImageDrawPixel(&icon,49,54,CLITERAL(Color){ 100, 134, 0, 255 });
	ImageDrawPixel(&icon,49,55,CLITERAL(Color){ 89, 120, 0, 255 });
	ImageDrawPixel(&icon,49,56,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,49,57,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,49,58,CLITERAL(Color){ 94, 125, 0, 255 });
	ImageDrawPixel(&icon,49,59,CLITERAL(Color){ 78, 119, 0, 255 });
	ImageDrawPixel(&icon,49,60,CLITERAL(Color){ 45, 67, 0, 255 });
	ImageDrawPixel(&icon,49,61,CLITERAL(Color){ 220, 216, 231, 27 });
	ImageDrawPixel(&icon,50,12,CLITERAL(Color){ 74, 90, 36, 227 });
	ImageDrawPixel(&icon,50,13,CLITERAL(Color){ 67, 106, 0, 255 });
	ImageDrawPixel(&icon,50,14,CLITERAL(Color){ 91, 122, 0, 255 });
	ImageDrawPixel(&icon,50,15,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,50,16,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,50,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,50,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,50,19,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,50,20,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,50,21,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,50,22,CLITERAL(Color){ 100, 134, 0, 255 });
	ImageDrawPixel(&icon,50,23,CLITERAL(Color){ 58, 96, 0, 255 });
	ImageDrawPixel(&icon,50,24,CLITERAL(Color){ 36, 54, 15, 252 });
	ImageDrawPixel(&icon,50,25,CLITERAL(Color){ 189, 184, 202, 68 });
	ImageDrawPixel(&icon,50,30,CLITERAL(Color){ 211, 231, 239, 20 });
	ImageDrawPixel(&icon,50,31,CLITERAL(Color){ 186, 197, 200, 66 });
	ImageDrawPixel(&icon,50,32,CLITERAL(Color){ 154, 153, 152, 115 });
	ImageDrawPixel(&icon,50,33,CLITERAL(Color){ 130, 119, 114, 149 });
	ImageDrawPixel(&icon,50,34,CLITERAL(Color){ 123, 104, 96, 169 });
	ImageDrawPixel(&icon,50,35,CLITERAL(Color){ 118, 94, 85, 180 });
	ImageDrawPixel(&icon,50,36,CLITERAL(Color){ 110, 85, 76, 182 });
	ImageDrawPixel(&icon,50,37,CLITERAL(Color){ 118, 97, 89, 170 });
	ImageDrawPixel(&icon,50,38,CLITERAL(Color){ 131, 118, 112, 156 });
	ImageDrawPixel(&icon,50,39,CLITERAL(Color){ 147, 143, 141, 127 });
	ImageDrawPixel(&icon,50,40,CLITERAL(Color){ 176, 183, 185, 77 });
	ImageDrawPixel(&icon,50,41,CLITERAL(Color){ 203, 222, 229, 31 });
	ImageDrawPixel(&icon,50,52,CLITERAL(Color){ 142, 140, 155, 113 });
	ImageDrawPixel(&icon,50,53,CLITERAL(Color){ 49, 66, 27, 239 });
	ImageDrawPixel(&icon,50,54,CLITERAL(Color){ 34, 65, 0, 255 });
	ImageDrawPixel(&icon,50,55,CLITERAL(Color){ 62, 99, 0, 255 });
	ImageDrawPixel(&icon,50,56,CLITERAL(Color){ 80, 117, 0, 255 });
	ImageDrawPixel(&icon,50,57,CLITERAL(Color){ 81, 119, 0, 255 });
	ImageDrawPixel(&icon,50,58,CLITERAL(Color){ 54, 93, 0, 255 });
	ImageDrawPixel(&icon,50,59,CLITERAL(Color){ 16, 41, 0, 255 });
	ImageDrawPixel(&icon,50,60,CLITERAL(Color){ 155, 155, 158, 113 });
	ImageDrawPixel(&icon,51,12,CLITERAL(Color){ 65, 84, 13, 246 });
	ImageDrawPixel(&icon,51,13,CLITERAL(Color){ 84, 123, 0, 255 });
	ImageDrawPixel(&icon,51,14,CLITERAL(Color){ 87, 117, 0, 255 });
	ImageDrawPixel(&icon,51,15,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,51,16,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,51,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,51,18,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,51,19,CLITERAL(Color){ 87, 118, 0, 255 });
	ImageDrawPixel(&icon,51,20,CLITERAL(Color){ 91, 121, 0, 255 });
	ImageDrawPixel(&icon,51,21,CLITERAL(Color){ 99, 135, 0, 255 });
	ImageDrawPixel(&icon,51,22,CLITERAL(Color){ 42, 78, 0, 255 });
	ImageDrawPixel(&icon,51,23,CLITERAL(Color){ 56, 68, 46, 221 });
	ImageDrawPixel(&icon,51,24,CLITERAL(Color){ 210, 204, 223, 45 });
	ImageDrawPixel(&icon,51,53,CLITERAL(Color){ 229, 222, 245, 19 });
	ImageDrawPixel(&icon,51,54,CLITERAL(Color){ 134, 136, 142, 123 });
	ImageDrawPixel(&icon,51,55,CLITERAL(Color){ 98, 111, 71, 194 });
	ImageDrawPixel(&icon,51,56,CLITERAL(Color){ 80, 99, 31, 234 });
	ImageDrawPixel(&icon,51,57,CLITERAL(Color){ 77, 97, 27, 236 });
	ImageDrawPixel(&icon,51,58,CLITERAL(Color){ 88, 101, 75, 192 });
	ImageDrawPixel(&icon,51,59,CLITERAL(Color){ 174, 171, 181, 87 });
	ImageDrawPixel(&icon,52,12,CLITERAL(Color){ 103, 109, 91, 177 });
	ImageDrawPixel(&icon,52,13,CLITERAL(Color){ 32, 69, 0, 255 });
	ImageDrawPixel(&icon,52,14,CLITERAL(Color){ 102, 138, 0, 255 });
	ImageDrawPixel(&icon,52,15,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,52,16,CLITERAL(Color){ 88, 118, 0, 255 });
	ImageDrawPixel(&icon,52,17,CLITERAL(Color){ 88, 119, 0, 255 });
	ImageDrawPixel(&icon,52,18,CLITERAL(Color){ 90, 120, 0, 255 });
	ImageDrawPixel(&icon,52,19,CLITERAL(Color){ 100, 135, 0, 255 });
	ImageDrawPixel(&icon,52,20,CLITERAL(Color){ 72, 110, 0, 255 });
	ImageDrawPixel(&icon,52,21,CLITERAL(Color){ 28, 57, 0, 255 });
	ImageDrawPixel(&icon,52,22,CLITERAL(Color){ 89, 96, 90, 179 });
	ImageDrawPixel(&icon,52,23,CLITERAL(Color){ 240, 233, 253, 12 });
	ImageDrawPixel(&icon,52,56,CLITERAL(Color){ 234, 229, 250, 8 });
	ImageDrawPixel(&icon,52,57,CLITERAL(Color){ 232, 227, 248, 9 });
	ImageDrawPixel(&icon,53,12,CLITERAL(Color){ 229, 224, 242, 21 });
	ImageDrawPixel(&icon,53,13,CLITERAL(Color){ 62, 71, 51, 217 });
	ImageDrawPixel(&icon,53,14,CLITERAL(Color){ 28, 62, 0, 255 });
	ImageDrawPixel(&icon,53,15,CLITERAL(Color){ 75, 113, 0, 255 });
	ImageDrawPixel(&icon,53,16,CLITERAL(Color){ 105, 143, 0, 255 });
	ImageDrawPixel(&icon,53,17,CLITERAL(Color){ 90, 127, 0, 255 });
	ImageDrawPixel(&icon,53,18,CLITERAL(Color){ 56, 92, 0, 255 });
	ImageDrawPixel(&icon,53,19,CLITERAL(Color){ 32, 62, 0, 255 });
	ImageDrawPixel(&icon,53,20,CLITERAL(Color){ 57, 72, 37, 227 });
	ImageDrawPixel(&icon,53,21,CLITERAL(Color){ 149, 148, 164, 104 });
	ImageDrawPixel(&icon,54,13,CLITERAL(Color){ 244, 238, 254, 7 });
	ImageDrawPixel(&icon,54,14,CLITERAL(Color){ 129, 131, 136, 128 });
	ImageDrawPixel(&icon,54,15,CLITERAL(Color){ 80, 94, 49, 215 });
	ImageDrawPixel(&icon,54,16,CLITERAL(Color){ 50, 67, 3, 242 });
	ImageDrawPixel(&icon,54,17,CLITERAL(Color){ 69, 85, 27, 236 });
	ImageDrawPixel(&icon,54,18,CLITERAL(Color){ 112, 123, 91, 178 });
	ImageDrawPixel(&icon,54,19,CLITERAL(Color){ 149, 149, 162, 103 });
	ImageDrawPixel(&icon,54,20,CLITERAL(Color){ 236, 229, 251, 9 });
	
	ImageResize(&icon,width,height);
	
	pTurtle->icon_width = width;
	pTurtle->icon_height = height;
	
	pTurtle->icon = LoadTextureFromImage(icon);
	UnloadImage(icon);
}


static void doDrawLineF(float x0, float y0, float x1, float y1) {
	ImageDrawLineEx(&pWorld->world_image,x0,y0,x1,y1,pTurtle->pen_size,pTurtle->pen_color);
}

void drawLine(int x0, int y0, int x1, int y1) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	ImageDrawLineEx(
		&pWorld->world_image,
		pWorld->origin_x+x0,
		pWorld->origin_y-y0,
		pWorld->origin_x+x1,
		pWorld->origin_y-y1,
		pTurtle->pen_size,
		pTurtle->pen_color
		);
	refreshWorld();
}

void initWorld(int width,int height){
	if (pWorld!=NULL)
		return;
	InitWindow(width,height,"Turtle World");
	SetTargetFPS(0);
	SetTraceLogLevel(LOG_WARNING);
	
	pWorld = (World*)malloc(sizeof(World));
	pWorld->width = width;
	pWorld->height = height;
	pWorld->back_color = WHITE;
	pWorld->is_rewind = false;
	pWorld->is_immediate = false;
	pWorld->origin_x = width/2;
	pWorld->origin_y = height/2;
	pWorld->use_background_image = false;
	pWorld->frame_count = 0;
	pWorld->window_should_close=false;
	pWorld->use_grid = false;
	pWorld->hide_grid = true;
	pWorld->hide_background_image = true;
	
	pTurtle=(Turtle*)malloc(sizeof(Turtle));
	pTurtle->pen_size = 1;
	pTurtle->pen_color = BLACK;
	pTurtle->x=pWorld->origin_x;
	pTurtle->y=pWorld->origin_y;
	pTurtle->orient = -90;
	pTurtle->move_speed = 100;
	pTurtle->is_show = true;
	pTurtle->is_pen_down = true;
	prepareTurtleOriginIcon();
	
	pWorld->world_image = GenImageColor(width,height,BLANK);
	createGrids(100,1,20,DARKRED,DARKCYAN);
}

void closeWorld(){
	if (pWorld==NULL)
		return;
	//pWorld->is_exit=true; 
	UnloadImage(pWorld->world_image);
	if (pWorld->use_background_image) {
		UnloadTexture(pWorld->background);
	}
	if (pWorld->use_grid) {
		UnloadTexture(pWorld->grids);
	}
	UnloadTexture(pTurtle->icon);
	CloseWindow();
	free(pWorld);
	free(pTurtle);
}

void waitClick() {
	if (pWorld==NULL)
		return;
	while(!pWorld->window_should_close) {
		interactWithUser();
		displayWorld();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			break;
		usleep(100000);
	}
}

void fd(double step)
{
	forward(step);
}

void forward(double step)
{
	double delta_x,delta_y;
	double x,y,old_x,old_y;
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	delta_x=BASE_STEP*cos(d2a(pTurtle->orient));
	delta_y=BASE_STEP*sin(d2a(pTurtle->orient));
	if (step<0)
	{
		delta_x=-delta_x;
		delta_y=-delta_y;
		step=-step;
	}
	
	if (!pWorld->is_immediate) {
		old_x=x=pTurtle->x;
		old_y=y=pTurtle->y;
		for (int i=0; i<step; i++)
		{
			if ((i+1)<step)
			{
				x+=delta_x;
				y+=delta_y;
			}
			else
			{
				x+=(step-i)*delta_x;
				y+=(step-i)*delta_y;
			}
			
			if (pWorld->is_rewind)
			{
				if (x<0)
				{
					x+=pWorld->width;
					old_x+=pWorld->width;
				}
				else if (x>pWorld->width)
				{
					x-=pWorld->width;
					old_x-=pWorld->width;
				}
				if (y<0)
				{
					y+=pWorld->height;
					old_y+=pWorld->height;
				}
				else if (y>pWorld->height)
				{
					y-=pWorld->height;
					old_y-=pWorld->height;
				}
			}
			if (pTurtle->is_pen_down)
			{
				doDrawLineF(
					old_x,
					old_y,
					x,
					y);
			}
			old_x=x;
			old_y=y;
			pTurtle->x=x;
			pTurtle->y=y;
			refreshWorld();
		}
	} else {
		x=pTurtle->x+step*delta_x;
		y=pTurtle->y+step*delta_y;
		if (pWorld->is_rewind) {
			while (x<0) {
				x+=pWorld->width;
			}
			while (x>pWorld->width) {
				x-=pWorld->width;
			}
			while (y<0) {
				y+=pWorld->height;
			}
			while (y>pWorld->height) {
				y-=pWorld->height;
			}
		}
		if (pTurtle->is_pen_down) {
			doDrawLineF(
				pTurtle->x,
				pTurtle->y,
				x,
				y);
		}
		pTurtle->x=x;
		pTurtle->y=y;
	}
	refreshWorld();
}
void bk(double step)
{
	backward(step);
}
void backward(double step)
{
	forward(-step);
}
void lt(double degree)
{
	leftTurn(degree);
}
void leftTurn(double degree)
{
	double origin_angle=pTurtle->orient;
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	if (!pWorld->is_immediate)
	{
		if (degree>0)
		{
			for (int i=0; i<degree; i+=2)
			{
				pTurtle->orient=origin_angle-i;
				refreshWorld();
			}
		}
		else
		{
			for (int i=0; i<-degree; i+=2)
			{
				pTurtle->orient=origin_angle+i;
				refreshWorld();
			}
		}
	}
	degree=degree-(int)degree/360*360;
	pTurtle->orient=origin_angle-degree;
	if (pTurtle->orient>360)
	{
		pTurtle->orient-=360;
	}
	if (pTurtle->orient<0)
	{
		pTurtle->orient+=360;
	}
	//prepareTurtleIcon();
	refreshWorld();
}
void rt(double degree)
{
	rightTurn(degree);
}
void rightTurn(double degree)
{
	lt(-degree);
}

void pd()
{
	penDown();
}

void penDown()
{
	if (pWorld==NULL)
		return;
	pTurtle->is_pen_down = true;
}

void penUp()
{
	if (pWorld==NULL)
		return;
	pTurtle->is_pen_down = false;
}

void pu()
{
	penUp();
}

void setPenDown(bool isPenDown) {
	if (pWorld==NULL)
		return;
	pTurtle->is_pen_down = isPenDown;
}

bool isPenDown() {
	if (pWorld==NULL)
		return false;
	return pTurtle->is_pen_down;
}

void clearScreen()
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	ImageClearBackground(&pWorld->world_image,BLANK);
	
	pTurtle->x=pWorld->origin_x;
	pTurtle->y=pWorld->origin_y;
	pTurtle->orient=-90;
	refreshWorld();
}

void cs()
{
	clearScreen();
}
void clear()
{
	clearScreen();
}

void home()
{
	int to_x,to_y;
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	to_x=pWorld->origin_x;
	to_y=pWorld->origin_y;
	if (pTurtle->is_pen_down)
	{
		doDrawLineF(
			pTurtle->x,
			pTurtle->y,
			to_x,
			to_y);
	}
	pTurtle->x=to_x;
	pTurtle->y=to_y;
	pTurtle->orient=-90;
	//prepareTurtleIcon();
	refreshWorld();
}

void setPenSize(int size)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pTurtle->pen_size=size;
};

int getPenSize(){
	if (pWorld==NULL)
		return 1;
	if (pWorld->window_should_close)
		return 1;
	return pTurtle->pen_size;
}

void setPenColor(Color color)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pTurtle->pen_color=color;
}

Color getPenColor() {
	if (pWorld==NULL)
		return BLACK;
	if (pWorld->window_should_close)
		return BLACK;
	return pTurtle->pen_color;
	
}

void setSpeed(int speed)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	if (speed>=1)
	{
		pTurtle->move_speed=speed;
	}
	else
	{
		pTurtle->move_speed=1;
	}
}

int getSpeed(){
	if (pWorld==NULL)
		return 1;
	if (pWorld->window_should_close)
		return 1;
	return pTurtle->move_speed;
	
}

void setRewind(bool isRewind)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;	
	pWorld->is_rewind=isRewind;
}

bool isRewind() {
	if (pWorld==NULL)
		return true;
	if (pWorld->window_should_close)
		return true;
	return pWorld->is_rewind;
}

void setImmediate(bool isImmediate)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pWorld->is_immediate=isImmediate;
}

bool isImmediate() {
	if (pWorld==NULL)
		return false;
	if (pWorld->window_should_close)
		return false;
	return pWorld->is_immediate;
}

void show()
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pTurtle->is_show=true;
	refreshWorld();
}

void hide()
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pTurtle->is_show=false;
	refreshWorld();
}

void setXY(double x, double y)
{
	pTurtle->x=pWorld->origin_x+x;
	pTurtle->y=pWorld->origin_y-y;
	refreshWorld();
}

double getX()
{
	if (pWorld==NULL)
		return 0;
	if (pWorld->window_should_close)
		return 0;
	return pTurtle->x-pWorld->origin_x;
}

double getY()
{
	if (pWorld==NULL)
		return 0;
	if (pWorld->window_should_close)
		return 0;
	return pWorld->origin_y-pTurtle->y;
}

void setAngle(double angle)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pTurtle->orient=-angle;
	refreshWorld();
}

void setHeading(double angle) {
	return setAngle(angle);
}

double getAngle()
{
	if (pWorld==NULL)
		return 0;
	if (pWorld->window_should_close)
		return 0;
	return -pTurtle->orient;
}

double getHeading() {
	return getAngle();
}

TurtleState getState()
{
	TurtleState state;
	if (pWorld==NULL)
		return state;
	if (pWorld->window_should_close)
		return state;
	state.x=getX();
	state.y=getY();
	state.angle=getAngle();
	state.is_pen_down = pTurtle->is_pen_down;
	state.pen_color = pTurtle->pen_color;
	state.pen_size = pTurtle->pen_size;
	return state;
}

void setState(TurtleState state)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	setXY(state.x,state.y);
	setAngle(state.angle);
	pTurtle->is_pen_down = state.is_pen_down;
	pTurtle->pen_color = state.pen_color;
	pTurtle->pen_size = state.pen_size;
}

void faceXY(double x,double y)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	x=pWorld->origin_x+x;
	y=pWorld->origin_y-y;
	double delta_x=x-pTurtle->x;
	double delta_y=-(y-pTurtle->y);
	double angle=atan2(delta_y,delta_x)/M_PI*180;
	turnTo(angle);
}

void turnTo(double angle)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	double turn_angle;
	turn_angle=-angle - pTurtle->orient ;
	while (turn_angle>180)
	{
		turn_angle=turn_angle-360;
	}
	while (turn_angle<-180)
	{
		turn_angle=360+turn_angle;
	}
	rt(turn_angle);
}
void gotoXY(double x, double y)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	faceXY(x,y);
	
	x=pWorld->origin_x+x;
	y=pWorld->origin_y-y;
	double delta_x=x-pTurtle->x;
	double delta_y=-(y-pTurtle->y);
	double step=sqrt(delta_x*delta_x+delta_y*delta_y);
	fd(step);
	pTurtle->x=x;
	pTurtle->y=y;
	refreshWorld();
}

void setOrigin(int x, int y)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	pWorld->origin_x=pWorld->origin_x+x;
	pWorld->origin_y=pWorld->origin_y-y;
	home();
	clearScreen();
}


void setCaption(const char* title)
{
	SetWindowTitle(title);
}

void setBackgroundImage(Image backImg)
{
	if (pWorld==NULL)
		return;
	if (pWorld->window_should_close)
		return;
	if (pWorld->use_background_image) {
		UnloadTexture(pWorld->background);
		pWorld->use_background_image = false;
	} 
	pWorld->background = LoadTextureFromImage(backImg);
	pWorld->use_background_image = true;
	refreshWorld();
}

int setBackgroundImageFile(const char* filename) {
	if (pWorld==NULL)
		return 0;
	if (pWorld->window_should_close)
		return 0;
	if (pWorld->use_background_image) {
		UnloadTexture(pWorld->background);
		pWorld->use_background_image = false;
	} 
	if (filename==NULL) 
		return 0;
	Image image = LoadImage(filename);
	setBackgroundImage(image);
	UnloadImage(image);
	return 0;
}

void setBackgroundColor(Color color)
{
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	if (!COLOR_EQUAL(color,pWorld->back_color)) {
		pWorld->back_color = color;
		refreshWorld();
	}
}

Color getBackgroundColor() {
	if (pWorld==NULL)
		return WHITE;
	if (pWorld->window_should_close)
		return WHITE;
	return pWorld->back_color;
}


void fillEllipse(int centerX, int centerY, int radiusX, int radiuxY, Color fillColor) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;	
	ImageFillEllipseEx(
		&pWorld->world_image,
		pWorld->origin_x+centerX,
		pWorld->origin_y-centerY,
		radiusX,
		radiuxY,
		fillColor
		);
	refreshWorld();
}

void fillCircle(int centerX, int centerY, int radius, Color fillColor) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;	
	ImageFillCircleEx(
		&pWorld->world_image,
		pWorld->origin_x+centerX,
		pWorld->origin_y-centerY,
		radius,
		fillColor
		);
	refreshWorld();
}

void drawCircle(int centerX, int centerY, int radius) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;	
	ImageDrawCircleEx(
		&pWorld->world_image,
		pWorld->origin_x+centerX,
		pWorld->origin_y-centerY,
		radius,
		pTurtle->pen_size,
		pTurtle->pen_color
		);
	refreshWorld();
}


void drawPoint(int x, int y) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;	
	ImageDrawPointEx(
		&pWorld->world_image,
		pWorld->origin_x+x,pWorld->origin_y-y,
		pTurtle->pen_size,
		pTurtle->pen_color);
	refreshWorld();
}

void drawEllipse(int centerX, int centerY, int radiusX,int radiusY){
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	ImageDrawEllipseEx(
		&pWorld->world_image,
		pWorld->origin_x+centerX,
		pWorld->origin_y-centerY,
		radiusX,
		radiusY,
		pTurtle->pen_size,
		pTurtle->pen_color
		);
	refreshWorld();	
}

void fill(Color fillColor) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	int x = round(pTurtle->x);
	int y = round(pTurtle->y);
	Color oc = GetImageColor(pWorld->world_image, x,y);
	IntQueue queueX;
	IntQueue queueY;
	IntQueue_init(&queueX);
	IntQueue_init(&queueY);
	IntQueue_push(&queueX,x);
	IntQueue_push(&queueY,y);
	int flagsSize=sizeof(int)*pWorld->width*pWorld->height;
	int * flags=(int*)malloc(flagsSize);
	memset(flags,0,flagsSize);
	while (!IntQueue_empty(&queueX)) {
		x = IntQueue_pop(&queueX);
		y = IntQueue_pop(&queueY);
		if (x<0 || x>=pWorld->width)
			continue;
		if (y<0 || y>=pWorld->height)
			continue;
		if (flags[y*pWorld->width+x]==1)
			continue;
		Color cc = GetImageColor(pWorld->world_image,x,y);
		if (!COLOR_EQUAL(oc, cc))
			continue;
		ImageDrawPixel(&pWorld->world_image,x,y,fillColor);
		flags[y*pWorld->width+x]=1;
		IntQueue_push(&queueX,x);IntQueue_push(&queueY,y-1);
		IntQueue_push(&queueX,x);IntQueue_push(&queueY,y+1);
		IntQueue_push(&queueX,x+1);IntQueue_push(&queueY,y);
		IntQueue_push(&queueX,x-1);IntQueue_push(&queueY,y);
	}
	free(flags);
	IntQueue_free(&queueX);
	IntQueue_free(&queueY);
	refreshWorld();
}


void fillToBorder(Color borderColor, Color fillColor) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	ImageFloodFill(&pWorld->world_image,
		round(pTurtle->x),round(pTurtle->y),
		borderColor,fillColor);
	refreshWorld();
}

void setVisible(bool visible){
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	if (visible!=pTurtle->is_show) {
		pTurtle->is_show=visible;
		refreshWorld();
	}
}
bool isVisible() {
	if (pWorld==NULL)
		return false;
	if (pWorld->window_should_close)
		return false;
	return pTurtle->is_show;
}

void removeGrids() {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	if (pWorld->use_grid) {
		UnloadTexture(pWorld->grids);
		displayWorld();
	}
}

void createGrids(int gridSize, int gridLineWidth, int labelFontSize, Color gridColorX, Color gridColorY) {
	if (pWorld==NULL)
		return ;
	if (pWorld->window_should_close)
		return ;
	if (gridSize<=0)
		return;
	if (pWorld->use_grid) {
		UnloadTexture(pWorld->grids);
		pWorld->use_grid=false;
	}
	pWorld->use_grid=true;
	Image image = GenImageColor(pWorld->width,pWorld->height,BLANK);
	int x1,y1,x2,y2;
	char buffer[100]; 
	x1=pWorld->origin_x;
	x2=pWorld->origin_x;
	while (x1>=0 || x2<pWorld->width) {
		if (x1>=0) {
			ImageDrawLineEx(&image, x1,0,x1,pWorld->height, gridLineWidth, gridColorX);
			sprintf(buffer,"%d",x1-pWorld->origin_x);
			if (x1< GetFontDefault().baseSize+10) {
				ImageDrawText(&image, buffer,GetFontDefault().baseSize+10 + gridLineWidth/2+10 ,gridLineWidth/2+10,labelFontSize,gridColorX);				
			} else {
				ImageDrawText(&image, buffer,x1 + gridLineWidth/2+10 ,gridLineWidth/2+10,labelFontSize,gridColorX);				
			}
		}
		if (x2<pWorld->width) {
			ImageDrawLineEx(&image, x2,0,x2,pWorld->height, gridLineWidth, gridColorX);			
			sprintf(buffer,"%d",x2-pWorld->origin_x);
			ImageDrawText(&image, buffer, x2 + gridLineWidth/2+10 ,gridLineWidth/2+10,labelFontSize,gridColorX);
		}
		x1-=gridSize;
		x2+=gridSize;
	}
	y1=pWorld->origin_y;
	y2=pWorld->origin_y;
	while (y1>=0 || y2<pWorld->height) {
		if (y1>=0) {
			ImageDrawLineEx(&image, 0,y1,pWorld->width, y1, gridLineWidth, gridColorY);
			sprintf(buffer,"%d",pWorld->origin_y-y1);
			if (y1 < GetFontDefault().baseSize+10) {
				ImageDrawText(&image, buffer, gridLineWidth/2+10 , GetFontDefault().baseSize+10  + gridLineWidth/2+10,labelFontSize,gridColorY);
			} else {
				ImageDrawText(&image, buffer, gridLineWidth/2+10 , y1+ gridLineWidth/2+10,labelFontSize,gridColorY);
			}
		}
		if (y2<pWorld->height) {
			ImageDrawLineEx(&image, 0,y2,pWorld->width, y2, gridLineWidth, gridColorY);			
			sprintf(buffer,"%d",pWorld->origin_y-y2);
			ImageDrawText(&image, buffer, gridLineWidth/2+10 , y2+ gridLineWidth/2+10,labelFontSize,gridColorY);
		}
		y1-=gridSize;
		y2+=gridSize;
	}
	pWorld->grids=LoadTextureFromImage(image);
	UnloadImage(image);
	displayWorld();
}

